#include <string.h>
#include "ParseAst.h"

Bnf::ParseAst::ParseAst()
{
}

Bnf::ParseAst::~ParseAst()
{
}

void Bnf::ParseAst::_get(Bnf::Tree *ast, std::string &uri, e_mod mode, const char **needed, bool takeNod)
{
	if (ast->getChildListSize() > 0)
	{
		bool res = false;

		if (takeNod == true)
			res = true;
		else if (mode == this->WHITE)
			res = this->_is_any_of(ast->getToken(), needed, takeNod);
		else if (res == this->BLACK)
			res = this->_is_not_any_of(ast->getToken(), needed, takeNod);

		if (res)
		{
			std::list<Bnf::Tree *>::const_iterator it = ast->getChildList().begin();
			std::list<Bnf::Tree *>::const_iterator ite = ast->getChildList().end();

			while (it != ite)
			{
				this->_get(*it, uri, mode, needed, takeNod);
				++it;
			}
		}
	}
	else
	{
		uri += ast->getToken();
	}
}

std::string Bnf::ParseAst::getByWhiteList(const Tree &ast, const char **needed)
{
	std::list<Bnf::Tree *>::const_iterator it = ast.getChildList().begin();
	std::list<Bnf::Tree *>::const_iterator ite = ast.getChildList().end();
	std::string uri;

	while (it != ite)
	{
		this->_get(*it, uri, this->WHITE, needed, false);
		++it;
	}

	return (uri);
}

std::string Bnf::ParseAst::getByBlackList(const Tree &ast, const char **needed)
{
	std::list<Bnf::Tree *>::const_iterator it = ast.getChildList().begin();
	std::list<Bnf::Tree *>::const_iterator ite = ast.getChildList().end();
	std::string uri;

	while (it != ite)
	{
		this->_get(*it, uri, this->BLACK, needed, false);
		++it;
	}

	return (uri);
}

const Bnf::Tree	*Bnf::ParseAst::getNode(const Tree &ast, const char **way)
{
	std::list<Bnf::Tree *>::const_iterator it = ast.getChildList().begin();
	std::list<Bnf::Tree *>::const_iterator ite = ast.getChildList().end();
	const Bnf::Tree   *begin = &(ast);
	unsigned int i = 0;

	while (it != ite)
	{
		if ((*it)->getToken() == way[i])
		{
			begin = (*it);
			it = begin->getChildList().begin();
			ite = begin->getChildList().end();
			++i;
			if (way[i] == NULL)
				return (begin);
		}
		else
			++it;
	}
	return (NULL);
}

bool Bnf::ParseAst::_is_any_of(const std::string &src, const char **needed, bool& tnode)
{
	for (unsigned int i = 0; needed[i] != NULL ;++i)
	{
		if (src == needed[i])
		{
			if (strcmp(needed[i], TAKENODE) && needed[++i] != NULL)
			{
				if (strcmp(needed[i], TAKENODE) == 0)
					tnode = true;
			}
			return (true);
		}
	}
	return (false);
}

bool Bnf::ParseAst::_is_not_any_of(const std::string &src, const char **needed, bool& tnode)
{
	for (unsigned int i = 0; needed[i] != NULL ;++i)
	{
		if (strcmp(needed[i], TAKENODE))
		{
			tnode = true;
		}
		if (src == needed[i])
		{
			return (false);
		}
	}
	return (true);
}
