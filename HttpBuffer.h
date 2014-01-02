#ifndef __HTTP_BUFFER2_H__
# define __HTTP_BUFFER2_H__

# include <string>
# include <sstream>
# include <fstream>
# include <stdlib.h>
# include "bnf/Parser.h"
# include "bnf/HttpData.h"
# include "ApiImpl/Transaction.hpp"
# include "Tree.h"
# include "HttpParser.h"

class HttpBuffer
{
public:
	enum e_status
	{
		E_END_REQUEST,
		E_CONTINUE_BODY,
		E_CONTINUE_HEADER,
		E_ERROR,
		E_HEADER_COMPLETE
	};

	enum e_step
	{
		E_HEADER,
		E_BODY
	};

public:
	HttpBuffer(zia::api::network::IEndPoint *, zia::api::network::IEndPoint *, int);
	~HttpBuffer(void);

	e_status                    parse(const char *, int);
	ApiImpl::Transaction*       getTransaction() const;
	void                        setClientSocket(zia::api::network::IEndPoint *);
	void                        setServerSocket(zia::api::network::IEndPoint *);
	void                        clear();
	const std::string&          getHeader() const;
	std::stringstream&          getBody();

private:
	e_status _bodyStep(const char *, int);
	e_status _endBodyStep();
	e_status _headerStep(const char *, int);
	e_status _endHeaderStep(const char *, size_t, int);

private:

	std::string                         _header;
	std::stringstream                   _body;
	e_step                              _step;
	int                                 _size;
	int                                 _hsize;
	ApiImpl::Transaction*               _transaction;
	zia::api::network::IEndPoint*       _clientEndP;
	zia::api::network::IEndPoint*       _serveurEndP;
	int                                 _port;
    Bnf::Tree*                          _ast;
};

#endif //!__HTTP_BUFFER_H__
