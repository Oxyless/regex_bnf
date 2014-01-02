#include "metaMap.h"

Bnf::metaMap::metaMap(const std::map<std::string, std::list<std::string> > &bnfMap) : data(bnfMap)
{

}

const Bnf::metaMap& Bnf::metaMap::operator=(const metaMap& right)
{
	this->ruleName = right.ruleName;
	this->beginList = right.beginList;
	this->endList = right.endList;
	return (*this);
}

void Bnf::metaMap::display()
{
	std::list<std::string>::const_iterator tmp = this->beginList;

	while (tmp != this->endList)
	{
		std::cout << "<" << *tmp << "> ";
		++tmp;
	}
	std::cout << std::endl;
}
