#ifndef __BNF_EXCEPTION_H__
#define __BNF_EXCEPTION_H__

#include <exception>
#include <string>

class	BnfException : public std::exception
{
public:
	virtual ~BnfException() throw();
	BnfException(std::string error);
	BnfException(std::string part1, std::string part2, std::string part3);
	const char* what() const throw();

private:
	std::string	_error;
};

#endif
