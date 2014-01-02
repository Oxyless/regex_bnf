#ifndef __TAKE_BLOCK__
#define __TAKE_BLOCK__

#include <vector>
#include "BnfEnum.h"
#include "BnfException.h"
#include "SymbolReconizer.h"
#include "metaMap.h"

namespace Bnf
{

class TakeBlock
{
	typedef void (Bnf::TakeBlock::*evaluateBlockFunction)(metaMap&, Bnf::e_symbol);

public:
    enum e_option
    {
        WITH_BLOCK_DELIMITERS,
        WITHOUT_BLOCK_DELIMITERS
    };

public:
	TakeBlock(void);
	~TakeBlock(void);

	Bnf::metaMap	take(metaMap bnfMap, e_option options = WITHOUT_BLOCK_DELIMITERS);

private:
	const std::string	_findCharClose(metaMap&, Bnf::e_symbol);
	void				_takeBlock(metaMap&, Bnf::e_symbol);
    void				_takeSpecialBlock(metaMap&, Bnf::e_symbol);
	void				_takeUnik(metaMap&, Bnf::e_symbol);
	void				_takeSpecial(metaMap&, Bnf::e_symbol);
	void				_takeLiteral(metaMap&, Bnf::e_symbol);
	void				_takeRule(metaMap&, Bnf::e_symbol);

	void				_symbolUnknown(metaMap&, Bnf::e_symbol);
	void				_incr(std::list<std::string>::const_iterator &);

	std::vector<evaluateBlockFunction>					_blockFunctions;
	Bnf::SymbolReconizer								_symbolReconizer;
	std::list<std::string>::const_iterator				_maxIt;
    e_option                                            _options;
};

}

#endif