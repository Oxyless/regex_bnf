#ifndef __BNF_TREE__
# define __BNF_TREE__

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <list>
#include "BnfEnum.h"

namespace Bnf
{

	class BnfTree
	{
	public:
		BnfTree(const std::string&, e_symbol, const std::string&);
		BnfTree(const std::string&, e_symbol);
		BnfTree(const std::string&);
		BnfTree(e_symbol);
		BnfTree();

		virtual ~BnfTree(void);
		
		e_symbol						getNodType() const;
		void							setNodType(e_symbol type);
		const std::string&				getNodName() const;
		void							setNodname(const std::string &);
		void							setLiteral(const std::string &);
		const std::string&				getLiteral() const;
        void                            setNbLoop(unsigned int nb);
        unsigned int                    getNbLoop();

		std::list<BnfTree *>::iterator	begin();
		std::list<BnfTree *>::iterator	end();

        void					remove();
        BnfTree *				popBack();
		void					add(BnfTree *);
		BnfTree					*top();

		void					display(unsigned int offset = 0) const;

	private:
		std::string				_nodName;
		e_symbol				_nodType;
		std::string				_literal;
        unsigned int            _nbLoop;

		std::list<BnfTree *>	_childList;

		/*
		**	Algorithm
		*/
		class		DisplayTreeAlgorithm
		{
		public:
			DisplayTreeAlgorithm(void);
			void	operator()(const BnfTree *) const;
			void	setOffset(unsigned int);

		private:
			unsigned int		_offset;
		};
	};

}

#endif
