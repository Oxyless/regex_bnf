#include <iostream>
#include <sstream>
#include "PreParser.h"

Bnf::PreParser::PreParser() : _bnfTree(Bnf::RULE)
{
	this->_symbolFunctions.resize(Bnf::ENUM_SYMBOL_SIZE);

	this->_symbolFunctions[Bnf::RULE] = &Bnf::PreParser::_preParseRule;
	this->_symbolFunctions[Bnf::LITERAL] = &Bnf::PreParser::_preParseLiteral;
	this->_symbolFunctions[Bnf::UNKNOWN] = &Bnf::PreParser::_symbolUnknown;
	this->_symbolFunctions[Bnf::WHITE_LIST] = &Bnf::PreParser::_preParseSpecial;
	this->_symbolFunctions[Bnf::BLACK_LIST] = &Bnf::PreParser::_preParseSpecial;
	this->_symbolFunctions[Bnf::OR] = &Bnf::PreParser::_preParseOr;
	this->_symbolFunctions[Bnf::UNTIL] = &Bnf::PreParser::_preParseSpecial;
	this->_symbolFunctions[Bnf::SKIP_BLANK] = &Bnf::PreParser::_preParseUnik;
    this->_symbolFunctions[Bnf::NUMBER] = &Bnf::PreParser::_preParseSpecialBlock;
	this->_symbolFunctions[Bnf::NOT_SKIP_BLANK] = &Bnf::PreParser::_preParseUnik;
	this->_symbolFunctions[Bnf::BRACKET] = &Bnf::PreParser::_preParseBlock;
	this->_symbolFunctions[Bnf::PARENTHESIS] = &Bnf::PreParser::_preParseBlock;
	this->_symbolFunctions[Bnf::SHARP] = &Bnf::PreParser::_preParseSpecialBlock;
	this->_symbolFunctions[Bnf::STAR] = &Bnf::PreParser::_preParseSpecialBlock;
}

Bnf::PreParser::~PreParser(void)
{
}

void Bnf::PreParser::run(t_bnfMap &bnfMap, const std::string &ruleName)
{
	if (!_exist(bnfMap, ruleName))
		throw (new BnfException("Bnf::PreParser::_preParseRule : the rule <", ruleName, "> doesn't exist."));

	metaMap		mBnfMap(bnfMap);

	mBnfMap.beginList			= bnfMap.find(ruleName)->second.begin();
	mBnfMap.endList				= bnfMap.find(ruleName)->second.end();
	mBnfMap.ruleName			= ruleName;
	this->_bnfTree.setNodname(ruleName);
	_bnfCache[ruleName] = &_bnfTree;
	_preParse(mBnfMap, _bnfTree);
}

void Bnf::PreParser::_preParse(metaMap mBnfMap, Bnf::BnfTree &bnfTree)
{
	Bnf::e_symbol symbol;

	while (mBnfMap.beginList != mBnfMap.endList)
	{
		symbol = this->_symbolReconizer.identify(*(mBnfMap.beginList));

		(this->*(this->_symbolFunctions[symbol]))(mBnfMap, bnfTree, symbol);
	}
}

void								Bnf::PreParser::_preParseSpecial(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol)
{
	BnfTree *leaf = new BnfTree(symbol);

	++mBnfMap.beginList;
	leaf->setLiteral((*mBnfMap.beginList).substr(1, ((*mBnfMap.beginList).size() - 2)));
	bnfTree.add(leaf);
	++mBnfMap.beginList;
}

void								Bnf::PreParser::_preParseBlock(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol)
{
	Bnf::metaMap mmap = this->_takeBlock.take(mBnfMap);

	bnfTree.add(new BnfTree(symbol));
	mBnfMap.beginList = mmap.endList;
    ++mBnfMap.beginList;

	_preParse(mmap, *bnfTree.top());
}

void								Bnf::PreParser::_preParseSpecialBlock(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol)
{
    bnfTree.add(new BnfTree(symbol));

    if (symbol == Bnf::NUMBER)
    {
        std::stringstream buffer;
        unsigned int      loop;

        buffer << (*mBnfMap.beginList);
        buffer >> loop;

         bnfTree.top()->setNbLoop(loop);
    }

    ++mBnfMap.beginList;

    Bnf::metaMap mmap = this->_takeBlock.take(mBnfMap, Bnf::TakeBlock::WITH_BLOCK_DELIMITERS);

    mBnfMap.beginList = mmap.endList;
    _preParse(mmap, *bnfTree.top());
}


void								Bnf::PreParser::_preParseOr(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol bnf_symbol)
{
    BnfTree *nod = bnfTree.popBack();

    bnfTree.add(new BnfTree(bnf_symbol));
    (*bnfTree.top()).add(nod);
    ++mBnfMap.beginList;

    Bnf::e_symbol symbol;
    bool          or_flag = true;
 
    while (mBnfMap.beginList != mBnfMap.endList)
    {
        symbol = this->_symbolReconizer.identify(*(mBnfMap.beginList));

        if (symbol == Bnf::OR && or_flag == false)
        {
            or_flag = true;
            ++mBnfMap.beginList;
        }
        else if (or_flag == true)
        {  
         (this->*(this->_symbolFunctions[symbol]))(mBnfMap, (*bnfTree.top()), symbol);
         or_flag = false;
        }
        else
          break ;
      
    }
}


void								Bnf::PreParser::_preParseLiteral(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol)
{
	BnfTree *leaf = new BnfTree(symbol);

	leaf->setLiteral((*mBnfMap.beginList).substr(1, ((*mBnfMap.beginList).size() - 2)));
	bnfTree.add(leaf);
	++mBnfMap.beginList;
}

void								Bnf::PreParser::_preParseUnik(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol)
{
	bnfTree.add(new BnfTree(symbol));

	++mBnfMap.beginList;
}

void								Bnf::PreParser::_preParseRule(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol)
{
	if (!_exist(mBnfMap.data, *(mBnfMap.beginList)))
		throw (new BnfException("Bnf::PreParser::_preParseRule : the rule <", *(mBnfMap.beginList),"> doesn't exist."));

	Bnf::metaMap mmap = mBnfMap;

	mmap.ruleName = *mBnfMap.beginList;
	mmap.beginList = mBnfMap.data.find(mmap.ruleName)->second.begin();
	mmap.endList = mBnfMap.data.find(mmap.ruleName)->second.end();	
	
	bnfTree.add(new BnfTree(mmap.ruleName, symbol));
	_bnfCache[mmap.ruleName] = bnfTree.top();
	_preParse(mmap, *bnfTree.top());

	++mBnfMap.beginList;
}

void Bnf::PreParser::_symbolUnknown(metaMap& mBnfMap, Bnf::BnfTree &, Bnf::e_symbol)
{
	throw (new BnfException("Bnf::PreParser::_preParse : the symbol <", *(mBnfMap.beginList),"> doesn't exist."));
}

bool Bnf::PreParser::_exist(t_bnfMap &bnfMap, const std::string &ruleName)
{
	return (bnfMap.find(ruleName) != bnfMap.end());
}

const Bnf::BnfTree&		Bnf::PreParser::getBnfTree()
{
	return (this->_bnfTree);
}

const std::map<const std::string, Bnf::BnfTree *>&		Bnf::PreParser::getBnfCache()
{
	return (this->_bnfCache);
}