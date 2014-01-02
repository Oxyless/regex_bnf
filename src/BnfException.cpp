#include "BnfException.h"

BnfException::~BnfException() throw()
{

}

BnfException::BnfException(std::string error) : _error(error)
{
}

BnfException::BnfException(std::string part1, std::string part2, std::string part3)
{
	_error = (part1 + part2 + part3);
}

const char	*BnfException::what() const throw()
{
	return (this->_error.c_str());
}
