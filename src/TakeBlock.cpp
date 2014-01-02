#include "TakeBlock.h"

Bnf::TakeBlock::TakeBlock(void)
{
	this->_blockFunctions.resize(Bnf::ENUM_SYMBOL_SIZE);

	this->_blockFunctions[Bnf::RULE] = &Bnf::TakeBlock::_takeRule;
	this->_blockFunctions[Bnf::LITERAL] = &Bnf::TakeBlock::_takeLiteral;
	this->_blockFunctions[Bnf::UNKNOWN] = &Bnf::TakeBlock::_symbolUnknown;
	this->_blockFunctions[Bnf::WHITE_LIST] = &Bnf::TakeBlock::_takeSpecial;
	this->_blockFunctions[Bnf::BLACK_LIST] = &Bnf::TakeBlock::_takeSpecial;
	this->_blockFunctions[Bnf::OR] = &Bnf::TakeBlock::_takeUnik;
	this->_blockFunctions[Bnf::UNTIL] = &Bnf::TakeBlock::_takeSpecial;
	this->_blockFunctions[Bnf::SKIP_BLANK] = &Bnf::TakeBlock::_takeUnik;
	this->_blockFunctions[Bnf::NOT_SKIP_BLANK] = &Bnf::TakeBlock::_takeUnik;
	this->_blockFunctions[Bnf::BRACKET] = &Bnf::TakeBlock::_takeBlock;
	this->_blockFunctions[Bnf::PARENTHESIS] = &Bnf::TakeBlock::_takeBlock;
	this->_blockFunctions[Bnf::SHARP] = &Bnf::TakeBlock::_takeSpecialBlock;
	this->_blockFunctions[Bnf::STAR] = &Bnf::TakeBlock::_takeSpecialBlock;
    this->_blockFunctions[Bnf::NUMBER] = &Bnf::TakeBlock::_takeUnik;
}

Bnf::TakeBlock::~TakeBlock(void)
{
}

Bnf::metaMap Bnf::TakeBlock::take(metaMap mBnfMap, e_option options)
{
	Bnf::e_symbol symbol;

	symbol = this->_symbolReconizer.identify(*(mBnfMap.beginList));
	this->_maxIt = mBnfMap.endList;
    this->_options = options;

	(this->*(this->_blockFunctions[symbol]))(mBnfMap, symbol);

	return (mBnfMap);
}

void			Bnf::TakeBlock::_takeBlock(metaMap& mBnfMap, Bnf::e_symbol symbol)
{
	const std::string &block_open = *mBnfMap.beginList;
	const std::string block_close = _findCharClose(mBnfMap, symbol);
	int cpt = 0;

	mBnfMap.endList = mBnfMap.beginList;
	do
	{
		if (mBnfMap.endList == this->_maxIt)
			throw (new BnfException("Bnf::TakeBlock::_incr : invalid block"));
		if (*mBnfMap.endList == block_open)
			++cpt;
		else if (*mBnfMap.endList == block_close)
			--cpt;
 
		_incr(mBnfMap.endList);
	}
	while (cpt > 0);
	
    if (this->_options == WITHOUT_BLOCK_DELIMITERS)
    {
	    _incr(mBnfMap.beginList);
        --mBnfMap.endList;
    }
}

void		    Bnf::TakeBlock::_takeSpecialBlock(metaMap& mBnfMap, Bnf::e_symbol)
{
    Bnf::e_symbol subSymbol;
    std::list<std::string>::const_iterator saveItPos = mBnfMap.beginList;
        
    _incr(mBnfMap.beginList);
   
    subSymbol = this->_symbolReconizer.identify(*(mBnfMap.beginList));
    mBnfMap = this->take(mBnfMap, this->_options);

    mBnfMap.beginList = saveItPos;
}

const std::string Bnf::TakeBlock::_findCharClose(metaMap& mBnfMap, Bnf::e_symbol symbol)
{
	std::string char_close;

	switch (symbol)
	{
	case Bnf::BRACKET :
		char_close = "]";
		break ;
	case Bnf::PARENTHESIS :
		char_close = ")";
		break ;
	default :
		throw (new BnfException("Bnf::PreParser::_preParseRule : the symbol <", *mBnfMap.beginList , "> can't be use as a block."));
		break ;
	}
	return (char_close);
}

void			Bnf::TakeBlock::_takeRule(metaMap& mBnfMap, Bnf::e_symbol)
{
	mBnfMap.endList = mBnfMap.beginList;
	_incr(mBnfMap.endList);
}

void			Bnf::TakeBlock::_takeUnik(metaMap& mBnfMap, Bnf::e_symbol)
{
	throw (new BnfException("Bnf::PreParser::_preParseRule : the symbol <", *mBnfMap.beginList , "> can't be use as a block."));
}

void			Bnf::TakeBlock::_takeSpecial(metaMap& mBnfMap, Bnf::e_symbol)
{
	mBnfMap.endList = mBnfMap.beginList;
	_incr(mBnfMap.endList);
	_incr(mBnfMap.endList);
}

void			Bnf::TakeBlock::_takeLiteral(metaMap& mBnfMap, Bnf::e_symbol)
{
	mBnfMap.endList = mBnfMap.beginList;
	_incr(mBnfMap.endList);
}

void Bnf::TakeBlock::_incr(std::list<std::string>::const_iterator &it)
{
	if (it == this->_maxIt)
		throw (new BnfException("Bnf::TakeBlock::_incr : invalid block"));
	else
		++it;
}


void			Bnf::TakeBlock::_symbolUnknown(metaMap& mBnfMap, Bnf::e_symbol)
{
	throw (new BnfException("Bnf::PreParser::_preParse : the symbol <", *(mBnfMap.beginList),"> doesn't exist."));
}