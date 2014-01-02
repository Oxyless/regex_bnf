#ifndef __METAMAP_H__
#define __METAMAP_H__

/* 
	Clement Bruchon -- 30 Mars 2010 -- brucho_c@epitech.net
*/

#include <iostream>
#include <string>
#include <map>
#include <list>

namespace Bnf
{
	class metaMap
	{
	public:
		metaMap(const std::map<std::string, std::list<std::string> > &);
		const metaMap& operator=(const metaMap&);
		void display();

		const std::map<std::string, std::list<std::string> > &data;

		std::string											 ruleName;
		std::list<std::string>::const_iterator				 beginList;
		std::list<std::string>::const_iterator				 endList;
	};
}

#endif
