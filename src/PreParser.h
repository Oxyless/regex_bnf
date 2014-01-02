#ifndef __PreParser_H__
#define __PreParser_H__

#include <vector>
#include <map>

#include "BnfException.h"
#include "SymbolReconizer.h"
#include "TakeBlock.h"
#include "BnfTree.h"
#include "metaMap.h"
#include "Lexer.h"

namespace Bnf
{

	class PreParser
	{
	public:
		typedef const std::map<std::string, std::list<std::string> >	t_bnfMap;
		typedef std::map<const std::string, Bnf::BnfTree *>				t_cacheMap;
		typedef void (Bnf::PreParser::*evaluateSymbolFunction)(metaMap&, Bnf::BnfTree &, Bnf::e_symbol);

	public:
		PreParser();
		~PreParser(void);

		void													run(t_bnfMap &bnfMap, const std::string &ruleName);
		const Bnf::BnfTree&										getBnfTree();
		const std::map<const std::string, Bnf::BnfTree *>&		getBnfCache();

	private:
		void													_preParse(metaMap mBnfMap, Bnf::BnfTree &ast);

		void													_preParseLiteral(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);
		void													_preParseUnik(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);
		void													_preParseRule(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);
		void													_preParseSpecial(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);
		void													_preParseBlock(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);
        void													_preParseSpecialBlock(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);
        void													_preParseOr(metaMap& mBnfMap, Bnf::BnfTree &bnfTree, Bnf::e_symbol symbol);

	private:
		bool													_exist(t_bnfMap &bnfMap, const std::string &ruleName);
		void													_symbolUnknown(metaMap &, Bnf::BnfTree &, Bnf::e_symbol);

	private:
		t_cacheMap												_bnfCache;
		Bnf::BnfTree											_bnfTree;
		Bnf::TakeBlock											_takeBlock;
		Bnf::SymbolReconizer									_symbolReconizer;

		std::vector<evaluateSymbolFunction>						 _symbolFunctions;
	};

}

#endif