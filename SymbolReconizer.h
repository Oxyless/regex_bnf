#ifndef __ISWHATS_H__
#define __ISWHATS_H__

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>

#include "BnfEnum.h"

namespace Bnf
{

class SymbolReconizer
{
public:
	SymbolReconizer(void);
	~SymbolReconizer(void);
	
	Bnf::e_symbol		identify(const std::string &word);

private:
	bool				_isRule(const std::string &word);
	bool				_isLiteral(const std::string &word);
	bool				_isNumber(const std::string &word);

private:
	/* ensembles */
	const std::string E_ALPHA_NUM;
	const std::string E_NUM;
	const std::string E_ALPHA;
	const std::string E_BLANKS;
};

}

#endif