#include "SymbolReconizer.h"

Bnf::SymbolReconizer::SymbolReconizer(void) :
E_ALPHA_NUM("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"),
E_NUM("0123456789"),
E_ALPHA("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"),
E_BLANKS(" \t")
{ 
}

Bnf::SymbolReconizer::~SymbolReconizer(void)
{
}

Bnf::e_symbol Bnf::SymbolReconizer::identify(const std::string &word)
{
	if (word == ">")
		return (Bnf::SKIP_BLANK);
	if (word == "=")
		return (Bnf::NOT_SKIP_BLANK);
	if (word == "$")
		return (Bnf::UNTIL);
	if (word == "|")
		return (Bnf::OR);
	if (word == "~")
		return (Bnf::WHITE_LIST);
	if (word == "!")
		return (Bnf::BLACK_LIST);
	if (word == "#")
		return (Bnf::SHARP);
	if (_isNumber(word))
		return (Bnf::NUMBER);
	if (word == "*")
		return (Bnf::STAR);
	if (word == "[")
		return (Bnf::BRACKET);
	if (word == "(")
		return (Bnf::PARENTHESIS);
	if (_isLiteral(word) == true)
		return (Bnf::LITERAL);
	if (_isRule(word) == true)
		return (Bnf::RULE);
	else
		return (Bnf::UNKNOWN);
}

bool	Bnf::SymbolReconizer::_isRule(const std::string &word)
{
	if (word.find_first_not_of(E_NUM) != word.npos &&
		word.find_first_not_of(E_ALPHA_NUM + "-_") == word.npos)
		return (true);
	return (false);
}

bool	Bnf::SymbolReconizer::_isLiteral(const std::string &word)
{
	if (word.size() > 1 && 
		word[0] == '"' && 
		word[word.size() - 1] == '"')
		return (true);
	else if (word.size() > 1 && 
		word[0] == '\'' && 
		word[word.size() - 1] == '\'')
		return (true);
	return (false);
}

bool	Bnf::SymbolReconizer::_isNumber(const std::string &word)
{
	if (word.find_first_not_of(E_NUM) == word.npos)
		return (true);
	return (false);
}