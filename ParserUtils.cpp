#include "Parser.h"

bool	Bnf::Parser::_skipBlanks(metaRequest &mRequest)
{
    bool flag = false;

	while (mRequest.pos != mRequest.fin &&
		_readChar(mRequest.data[mRequest.pos], mRequest.separators))
    {
		++mRequest.pos;
        flag = true;
    }
    return (flag);
}

bool	Bnf::Parser::_readChar(const char val, const char needed)
{
	if (val == needed)
		return (true);
	return (false);
}

bool	Bnf::Parser::_readChar(const char val, const std::string &needed)
{
	std::string	tmp;

	tmp += val;
	if (tmp.find_first_not_of(needed) == tmp.npos)
		return (true);
	return (false);
}