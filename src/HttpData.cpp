#include "HttpData.h"
#include <stdlib.h>

Http::HttpData::HttpData(const Bnf::Tree &ast) :  _contentLenght(0), _ast(ast)
{
}

Http::HttpData::~HttpData(void)
{
}

std::string Http::HttpData::getMethod()
{
	const char *needed[] = {"Request-Line",  "Method",  NULL};

	return (this->getByWhiteList(this->_ast, needed));
}

std::string Http::HttpData::getUriPath()
{
	const char *needed[] = {"Request-Line",  "Request-URI", "abs-path", TAKENODE, \
		"absoluteURI", "hier-part", "net-path", "nod",  NULL};

	return (this->getByWhiteList(this->_ast, needed));
}

std::string Http::HttpData::getUriProtocol()
{
	const char *needed[] = {"Request-Line", "Request-URI", "absoluteURI", "scheme", NULL};

	return (this->getByWhiteList(this->_ast, needed));
}

std::string Http::HttpData::getUriDomain()
{
	const char *needed[] = {"Request-Line",  "Request-URI", "authority", "absoluteURI", \
		"hier-part", "net-path", "nod", "server", "reg-name", "hostport", TAKENODE, NULL};

	return (this->getByWhiteList(this->_ast, needed));
}

std::string Http::HttpData::getUri()
{
	const char *needed[] = {"Request-Line",  "Request-URI", TAKENODE, NULL};

	return (this->getByWhiteList(this->_ast, needed));
}

std::string Http::HttpData::getVersion()
{
	const char *needed[] = {"Request-Line",  "HTTP-Version", "MAJORMINOR",  TAKENODE, NULL};

	return (this->getByWhiteList(this->_ast, needed));
}

std::map<std::string, std::string>	 Http::HttpData::getHeaders()
{
	std::map<std::string, std::string> mapHEader;
	const char *way[] =  {"Headers", NULL};

	const Bnf::Tree *begin = this->getNode(this->_ast, way);
	if (!begin)
		return (mapHEader);	

	std::list<Bnf::Tree *>::const_iterator it = begin->getChildList().begin();
	std::list<Bnf::Tree *>::const_iterator ite = begin->getChildList().end();

	while (it != ite)
	{
        std::string tmp = (*it)->top()->getToken();
        ++it;
        ++it;
        mapHEader[tmp] = (*it)->top()->getToken();
        ++it;
        ++it;
	}

	return (mapHEader);	
}

ApiImpl::Request								 Http::HttpData::getRequest()
{
	ApiImpl::Request newRequest;

	newRequest.setUri(this->getUri());
	newRequest.setMethod(this->getMethod());
	newRequest.setPath(this->getUriPath());
	newRequest.setVersion(this->getVersion());
	this->getHeaders(&newRequest.getHeaders());

	return (newRequest);
}

void								 Http::HttpData::getRequest(zia::api::http::IRequest &getReq)
{
	getReq.setUri(this->getUri());
	getReq.setMethod(this->getMethod());
	getReq.setPath(this->getUriPath());
	getReq.setVersion(this->getVersion());
	this->getHeaders(&getReq.getHeaders());
}

void								 Http::HttpData::getHeaders(zia::api::http::IHeader *head)
{
	const char *way[] =  {"Headers", NULL};

	const Bnf::Tree *begin = this->getNode(this->_ast, way);

	std::list<Bnf::Tree *>::const_iterator it = begin->getChildList().begin();
	std::list<Bnf::Tree *>::const_iterator ite = begin->getChildList().end();

	_contentLenght = 0;
	while (it != ite)
	{
        std::string tmp = (*it)->top()->getToken();
        ++it;
        ++it;
        head->setValue(tmp, (*it)->top()->getToken());
        ++it;
        ++it;
	}

    /* // debug Headers
    std::map<zia::api::http::IHeader::Key, zia::api::http::IHeader::Value> tmpMap = head->getAsMap();
    std::map<zia::api::http::IHeader::Key, zia::api::http::IHeader::Value>::const_iterator ita = head->getAsMap().begin();
    std::map<zia::api::http::IHeader::Key, zia::api::http::IHeader::Value>::const_iterator iteb = head->getAsMap().end();

    while (ita != iteb)
    {
        std::cout << (*ita).first << " - " << (*ita).second << std::endl;
        ++ita;
    }
    */
       

	if (head->getAsMap().find("content-length") != head->getAsMap().end())
		_contentLenght = atoi((*(head->getAsMap().find("content-length"))).second.c_str());
}

/* need call to getheaders before */
unsigned int							 Http::HttpData::getLenght() 
{
	return (this->_contentLenght);
}
