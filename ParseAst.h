#ifndef __PARSE_AST__
# define __PARSE_AST__

#include <iostream>
#include "Tree.h"

#define TAKENODE "\r"

namespace Bnf
{
	class ParseAst
	{
		enum e_mod
		{
			WHITE,
			BLACK
		};

	public:
		ParseAst();
		~ParseAst(void);

		std::string getByWhiteList(const Tree &ast, const char **needed);
		std::string getByBlackList(const Tree &ast, const char **needed);
		const Tree	*getNode(const Tree &ast, const char **way);

	private:
		void _get(Bnf::Tree *, std::string &, e_mod, const char **, bool);
		bool _is_any_of(const std::string &, const char **tval, bool&);
		bool _is_not_any_of(const std::string &, const char **tval, bool&);

	};

}

#endif
