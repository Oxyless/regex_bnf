#ifndef __META_REQUEST_H__
#define __META_REQUEST_H__

/* 
	Clement Bruchon -- 30 Mars 2010 -- brucho_c@epitech.net
*/

#include <string>
#include <map>
#include <list>

namespace Bnf
{
	class metaRequest
	{
	public:
		metaRequest(const std::string &);

		const std::string &data;
		unsigned int	  pos;
		unsigned int	  fin;

		std::string		  separators;
	};
}

#endif
