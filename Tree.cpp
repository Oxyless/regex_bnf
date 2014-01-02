#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Tree.h"

Bnf::Tree::Tree(void)
{
}

Bnf::Tree::Tree(std::string tocken) : _tocken(tocken)
{
}

Bnf::Tree::Tree(const Tree & right)
{
	*this = right;
}

const std::string &Bnf::Tree::getToken() const
{
	return (this->_tocken);
}

Bnf::Tree::~Tree(void)
{
	std::list<Tree *>::iterator	ib	= this->_childList.begin();
	std::list<Tree *>::iterator	ibe	= this->_childList.end();

	for (; ib != ibe; ++ib)
		delete *ib;
}

const Bnf::Tree &	Bnf::Tree::operator=(const Bnf::Tree & right)
{
	if (this != &right)
	{
		/* Copy Attributs */
	}
	return *this;
}

void					Bnf::Tree::display(unsigned int offset) const
{
	std::cout <<  std::setfill(' ') << std::setw(offset) << "-" << this->_tocken << std::endl;

	if (this->_childList.empty())
		return ;

	Bnf::DisplayTreeAlgorithm	displayAlgo;

	displayAlgo.setOffset(offset + 2);
	std::for_each(this->_childList.begin(), this->_childList.end(), displayAlgo);
}

void					Bnf::Tree::add(Tree * child)
{
	this->_childList.push_back(child);
}

void					Bnf::Tree::remove()
{
	if (this->_childList.size() > 0)
	{
		delete *(--(this->_childList.end()));
		this->_childList.pop_back();
	}
}

Bnf::Tree			*Bnf::Tree::top()
{
	if (this->_childList.size() > 0)
		return (*(--(this->_childList.end())));
	return (NULL);
}

unsigned int	Bnf::Tree::getChildListSize()
{
	return (this->_childList.size());
}

void			Bnf::Tree::setToken(const std::string &token)
{
	this->_tocken = token;
}

const std::list<Bnf::Tree *>	&Bnf::Tree::getChildList() const
{
	return (this->_childList);
}

/*
** Algorithm
*/
Bnf::DisplayTreeAlgorithm::DisplayTreeAlgorithm(void) : _offset(0)
{
}

void					Bnf::DisplayTreeAlgorithm::operator()(const Bnf::Tree * node) const
{
	node->display(this->_offset);
	return ;
}

void					Bnf::DisplayTreeAlgorithm::setOffset(unsigned int offset)
{
	this->_offset = offset;
}
