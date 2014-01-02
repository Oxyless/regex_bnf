#ifndef __HTTP_DATA__
#define __HTTP_DATA__

#include <map>
#include <string>
#include <iostream>
#include "ParseAst.h"
#include "Tree.h"
#include "../ApiImpl/Request.hpp"

namespace Http
{
	class HttpData : public Bnf::ParseAst
	{
	public:
		HttpData(const Bnf::Tree &);
		~HttpData(void);

	/* Uri */
		std::string							 getMethod();
		std::string							 getUriDomain();
		std::string							 getUriProtocol();
		std::string							 getUriPath();
		std::string							 getUri();
		std::string							 getVersion();
		unsigned int						 getLenght();
		std::map<std::string, std::string>	 getHeaders();
		void								 getHeaders(zia::api::http::IHeader *head);
		ApiImpl::Request					 getRequest();
		void								 getRequest(zia::api::http::IRequest &);

	private:
		unsigned int _contentLenght;
		const Bnf::Tree &_ast;
	};

}

#endif
