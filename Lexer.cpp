#include <stdlib.h>
#include "Lexer.h"
#include "BnfException.h"

Bnf::Lexer::Lexer(void) : BLOCK_SIZE(512)
{
	this->_status = Bnf::Lexer::UNKNOWN;
	this->addSeparators(" ");
	this->addSeparators("\n");
    this->setSpecials("()[]#*!~|$:=>");
	this->_saveSeparators = false;
	this->_listEmpty = true;
	this->_mapEmpty = true;
	this->_backSlash = 0;
	this->_numFlag = false;
}

Bnf::Lexer::~Lexer(void)
{
    if (this->_stream.is_open())
        this->_stream.close();
}

bool Bnf::Lexer::run(const char *fileName)
{
	if (openStream(fileName) == false)
		return (false);
	this->createTokenList();
	this->createBnfMap();

    if (this->_stream.is_open())
        this->_stream.close();
	return (true);
}

bool Bnf::Lexer::openStream(const char *fileName)
{
	this->_stream.open(fileName);

	if (this->_stream.is_open())
		return (true);
	return (false);
}

bool Bnf::Lexer::createBnfMap()
{
	std::list<std::string>::iterator it;
	std::list<std::string>::iterator ite;
	std::string						 current_key;

	if (this->_listEmpty && this->_stream.is_open())
		this->createTokenList();

	if (this->_mapEmpty == false)
		return (false);

	it = this->_tokenList.begin();
	ite = this->_tokenList.end();;
	std::string &last = *it;
    bool        flag = false;
	while (it != ite)
	{
		if (*it == ":")
		{
            if (++(it) != ite && *it == "=")
            {
			    current_key = last;
                flag = true;
            }
            else
                throw (new BnfException("Bnf::Lexer::createBnfMap: the symbol <:> can't be alone."));
		}
        else if (flag == true)
            flag = false;
		else if (it != this->_tokenList.begin())
			this->_tokenMap[current_key].push_back(last);

		last = *it;
		++it;
	}
	this->_tokenMap[current_key].push_back(last);
	this->clearList();
	this->_mapEmpty = false;
	return (true);
}

bool Bnf::Lexer::createTokenList()
{
	std::string	str;
	int	 nb_read = 0;

	str.resize(BLOCK_SIZE + 1);
	while (this->_stream.eof() == false && this->_stream.good())
	{
		this->_stream.read(&str[0], BLOCK_SIZE);
		nb_read = this->_stream.gcount();
		for (int i = 0; i < nb_read; ++i)
		{
			this->_getStatus(str[i]);
			if (this->_status == UNKNOWN)
				this->_getUnknown(str[i--]);
			else if (this->_backSlash != 2)
				_token += str[i];
		}
	}
	this->_getStatus(0);
	this->_listEmpty = false;
	return (true);
}

void Bnf::Lexer::_getStatus(const char key)
{
	this->_getBackslash(key);

	if (this->_backSlash != 2)
	{
		switch (this->_status)
		{
			case SPECIAL:
				this->_getSpecial(key);
				break ;
			case SEPARATOR:
				this->_getSeparator(key);
				break ;
			case TOKEN:
				this->_getToken(key);
				break ;
			case DOUBLE_QUOTE:
				this->_getDoubleQuote(key);
				break ;
			case QUOTE:
				this->_getQuote(key);
				break ;
			case COMMENT:
				this->_getComment(key);
				break ;
			case UNKNOWN:
			  break;
		}
	}
}

void Bnf::Lexer::_getBackslash(const char key)
{
	static unsigned int pos = 0;

	if (this->_backSlash != 0 && 
		(key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' ||
		key == '6' || key == '7' || key == '8' || key == '9'))
	{
		this->_numFlag = true;
		this->_backSlash = 2;
		this->_num += key;
		return ;
	}
	else if (this->_numFlag == true && 
		!(key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' ||
		key == '6' || key == '7' || key == '8' || key == '9'))
	{
		char tmp = char(atoi(this->_num.c_str()));
		this->_token.erase(pos);
		this->_token += tmp;
		this->_num.clear();
		this->_numFlag = false;
		this->_backSlash = 0;
		return (this->_getBackslash(key));
	}

	if (this->_backSlash > 0)
		--this->_backSlash;
	if (key == '\\' && this->_backSlash == 0)
	{
		this->_backSlash = 2;
		pos = _token.size();
	}

}

void Bnf::Lexer::_getUnknown(const char key)
{
	if (key == ';')
		this->_status = COMMENT;
	else if (this->_isSpecial(key) == true)
		this->_status = SPECIAL;
	else if (key == '"')
		this->_status = DOUBLE_QUOTE;
	else if (key == '\'')
		this->_status = QUOTE;
	else if (this->_isSeparator(key) == true)
		this->_status = SEPARATOR;
	else
		this->_status = TOKEN;
}

void	Bnf::Lexer::_getSpecial(const char )
{
	static int counter = 0;
	
	counter++;
	if (counter == 2)
	{
		this->_saveToken();
		this->_status = UNKNOWN;
		counter = 0;
	}
}

void	Bnf::Lexer::_getSeparator(const char key)
{
	if (this->_isSeparator(key) == false)
	{
		if (this->_saveSeparators == true)
			this->_saveToken();
		this->_token.clear();
		this->_status = UNKNOWN; 
	}
}

void	Bnf::Lexer::_getToken(const char key)
{
	if (this->_isSeparator(key) == true || this->_isSpecial(key) == true || key == 0)
	{
		this->_saveToken();
		this->_status = UNKNOWN; 
	}
}

void Bnf::Lexer::_getComment(const char key)
{
	if (key == '\n')
	{
		this->_token.clear();
		this->_status = UNKNOWN;
	}
}

void Bnf::Lexer::_getDoubleQuote(const char key)
{
	static int counter = 0;

	if (counter == 2)
	{
		this->_saveToken();
		this->_status = UNKNOWN;
		counter = 0;
	}
	if (this->_backSlash == 0 && key == '"')
		++counter;
}

void Bnf::Lexer::_getQuote(const char key)
{
	static int counter = 0;

	if (counter == 2)
	{
		this->_saveToken();
		this->_status = UNKNOWN;
		counter = 0;
	}
	if (this->_backSlash == 0 && key == '\'')
		++counter;
}

bool	Bnf::Lexer::_isSeparator(const char key)
{
	std::string	tmp;

	tmp += key;
	return (tmp.find_first_not_of(this->_separators) == tmp.npos ? true : false);
}

bool	Bnf::Lexer::_isSpecial(const char key)
{
	std::string	tmp;

	tmp += key;
	return (tmp.find_first_not_of(this->_specials) == tmp.npos ? true : false);
}

void	Bnf::Lexer::_saveToken()
{
	this->_tokenList.push_back(this->_token);
	this->_token.clear();
}

void Bnf::Lexer::setSeparators(const std::string &separators)
{
	this->_separators = separators;
}

void Bnf::Lexer::addSeparators(const std::string &separators)
{
	this->_separators += separators;
}

const std::string &Bnf::Lexer::getSeparators(void)
{
	return (this->_separators);
}

void						   Bnf::Lexer::setSaveSeparators(bool saveSeparators)
{
	this->_saveSeparators = saveSeparators;
}

void							Bnf::Lexer::setSpecials(const std::string &specials)
{
	this->_specials = specials;
}

void							Bnf::Lexer::addSpecial(const std::string &specials)
{
	this->_specials += specials;
}

void	Bnf::Lexer::displayList()
{
	std::list<std::string>::iterator it = this->_tokenList.begin();
	std::list<std::string>::iterator ite = this->_tokenList.end();

	std::cout << "Display list:" << std::endl;
	while (it != ite)
	{
		std::cout << "" << *it << " ";
		++it;
	}
	std::cout << std::endl;
}

void	Bnf::Lexer::displayMap()
{
	std::map<std::string, std::list<std::string> >::iterator it1 = this->_tokenMap.begin();
	std::map<std::string, std::list<std::string> >::iterator ite2 = this->_tokenMap.end();
	
	std::cout << "Display map:" << std::endl;
	while (it1 != ite2)
	{
		std::list<std::string>::iterator it = (*it1).second.begin();
		std::list<std::string>::iterator ite = (*it1).second.end();

		std::cout << (*it1).first << " => ";
		while (it != ite)
		{
			std::cout << "" << *it << " ";
			++it;
		}
		std::cout << std::endl;
		++it1;
	}
}

const std::list<std::string>	&Bnf::Lexer::getTokenList()
{
	return (this->_tokenList);
}

const std::map<std::string, std::list<std::string> >	&Bnf::Lexer::getTokenMap()
{
	return (this->_tokenMap);
}

bool Bnf::Lexer::isMapEmpty()
{
	return (this->_mapEmpty);
}

bool Bnf::Lexer::isListEmpty()
{
	return (this->_listEmpty);	
}

void Bnf::Lexer::clearMap()
{
	this->_tokenMap.clear();
	this->_mapEmpty = true;
}

void Bnf::Lexer::clearList()
{
	this->_tokenList.clear();
	this->_listEmpty = true;
}

void Bnf::Lexer::reinit()
{
	this->clearList();
	this->clearMap();

	this->_status = Bnf::Lexer::UNKNOWN;
	this->addSeparators(" ");
	this->addSeparators("\n");
	this->_saveSeparators = false;
	this->_listEmpty = true;
	this->_mapEmpty = true;
	this->_backSlash = 0;
	this->_numFlag = false;
}
