#include "BnfTree.h"

/* constructeurs */
Bnf::BnfTree::BnfTree(const std::string&nodName, Bnf::e_symbol nodType, const std::string& terminal) : _nodName(nodName), _nodType(nodType), _literal(terminal) {}
Bnf::BnfTree::BnfTree(const std::string& nodName, Bnf::e_symbol nodType) : _nodName(nodName), _nodType(nodType) {}
Bnf::BnfTree::BnfTree(const std::string& nodName) : _nodName(nodName) {}
Bnf::BnfTree::BnfTree(Bnf::e_symbol nodType) : _nodType(nodType) {}
Bnf::BnfTree::BnfTree() {}

/* destructeurs */
Bnf::BnfTree::~BnfTree() {}

/* getteurs / setteurs */
Bnf::e_symbol Bnf::BnfTree::getNodType() const
{
	return (this->_nodType);
}

void		Bnf::BnfTree::setNodType(e_symbol type)
{
	this->_nodType = type;
}

const std::string&	Bnf::BnfTree::getNodName() const
{
	return (this->_nodName);
}

void							Bnf::BnfTree::setNodname(const std::string &newName)
{
	this->_nodName = newName;
}

void				Bnf::BnfTree::setLiteral(const std::string &lit)
{
	this->_literal = lit;
}

const std::string&  Bnf::BnfTree::getLiteral() const
{
	return (this->_literal);
}

void                 Bnf::BnfTree::setNbLoop(unsigned int nb)
{
    this->_nbLoop = nb;
}

unsigned int         Bnf::BnfTree::getNbLoop()
{
   return (this->_nbLoop);
}

void			Bnf::BnfTree::add(BnfTree *child)
{
	this->_childList.push_back(child);
}

Bnf::BnfTree	*Bnf::BnfTree::top()
{
	if (this->_childList.size() > 0)
		return (*(--(this->_childList.end())));
	return (NULL);
}

std::list<Bnf::BnfTree *>::iterator	Bnf::BnfTree::begin()
{
	return (this->_childList.begin());
}

std::list<Bnf::BnfTree *>::iterator	Bnf::BnfTree::end()
{
	return (this->_childList.end());
}

void						Bnf::BnfTree::remove()
{
    if (this->_childList.size() > 0)
    {
        delete *(--(this->_childList.end()));
        this->_childList.pop_back();
    }
}

Bnf::BnfTree *				Bnf::BnfTree::popBack()
{
    BnfTree *btop = this->top();

    this->_childList.pop_back();
    return (btop);
}

void					Bnf::BnfTree::display(unsigned int offset) const
{
	std::cout <<  std::setfill(' ') << std::setw(offset) << "-" << this->_nodName << " (" << this->_nodType << ") " << this->_literal << std::endl;

	if (this->_childList.empty())
		return ;

	Bnf::BnfTree::DisplayTreeAlgorithm	displayAlgo;

	displayAlgo.setOffset(offset + 2);
	std::for_each(this->_childList.begin(), this->_childList.end(), displayAlgo);
}

/*
** Algorithm
*/
Bnf::BnfTree::DisplayTreeAlgorithm::DisplayTreeAlgorithm(void) : _offset(0)
{
}

void					Bnf::BnfTree::DisplayTreeAlgorithm::operator()(const Bnf::BnfTree * node) const
{
	node->display(this->_offset);
	return ;
}

void					Bnf::BnfTree::DisplayTreeAlgorithm::setOffset(unsigned int offset)
{
	this->_offset = offset;
}

