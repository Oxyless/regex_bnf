#include "HttpBuffer.h"
#include "ResponseManager.h"
#include "Utils/HttpConfHandler.hpp"
#include "Uri.h"

HttpBuffer::HttpBuffer(zia::api::network::IEndPoint *clientEndP, zia::api::network::IEndPoint *serveurEndP, int port) :
  _body(std::ios_base::binary | std::ios_base::in | std::ios_base::out),
  _step(E_HEADER),
  _transaction(NULL),
  _clientEndP(clientEndP),
  _serveurEndP(serveurEndP),
  _port(port),
  _ast(NULL)
{
}

HttpBuffer::~HttpBuffer(void)
{
  this->clear();
}

HttpBuffer::e_status HttpBuffer::parse(const char *buff, int nb_read)
{
  e_status status = this->E_ERROR;

  if (this->_step == this->E_HEADER)
    status = _headerStep(buff, nb_read);
  else
    status = _bodyStep(buff, nb_read);
  return (status);
}

HttpBuffer::e_status HttpBuffer::_headerStep(const char *buff, int nb_read)
{
  e_status status = this->E_ERROR;
  size_t pos;

  _hsize = _header.size();
  _header.append(buff, nb_read);
  pos = _header.find("\r\n\r\n");

  if (pos == _header.npos)
    {
      _header += buff;
      if (_header.size() < 1001706)
	status = this->E_CONTINUE_HEADER;
      else
	status = this->E_ERROR;
    }
  else
    {

      _header.resize(pos + 4);

      status = _endHeaderStep(buff, pos, nb_read);
    }
  return (status);
}

HttpBuffer::e_status HttpBuffer::_endHeaderStep(const char *buff, size_t , int nb_read)
{
  e_status status = this->E_ERROR;
  _ast = httpParser::getInstance().parse("Request", _header);

  if (_ast == NULL)
      status = this->E_ERROR;
  else
    {
      Uri				address;
      Http::HttpData	p_data(*_ast);

      _transaction = new ApiImpl::Transaction(this->_clientEndP, this->_serveurEndP);
      p_data.getRequest(_transaction->getRequest());

      HttpConfig *tmp = HttpConfHandler::getInstance()->getConf(_port);
      if (tmp)
	  address.setRootPath(tmp->getRoot());
      address.setPath(_transaction->getRequest().getPath());
      _transaction->getRequest().setPath(address.getAbsolutePath());
      _transaction->getResponse().setVersion(_transaction->getRequest().getVersion());
	  _transaction->getResponse().getHeaders().setValue("Content-Type", "text/plain");

      this->_size = p_data.getLenght();

      char ctmp[4096];
      unsigned int j = 0;
      unsigned int i = (_header.size() - _hsize);
      unsigned int mSize = nb_read - (_header.size() - _hsize);

      for (; j <= mSize ;++j)
        {
          ctmp[j] = buff[i];
          ++i;
        }
      if (j > 0)
        this->_body.write(ctmp, j - 1);
      this->_step = this->E_BODY;
	  _size -= (nb_read - (_header.size() - _hsize));

      status = _endBodyStep();
      if (status == this->E_CONTINUE_BODY)
	status = this->E_HEADER_COMPLETE;
    }
  return (status);
}


HttpBuffer::e_status HttpBuffer::_bodyStep(const char *buff, int nb_read)
{
  e_status status = this->E_ERROR;

  this->_body.write(buff, nb_read);
  _size -= nb_read;
  status = _endBodyStep();
  return (status);
}

HttpBuffer::e_status HttpBuffer::_endBodyStep()
{
  e_status status = this->E_ERROR;

  if (_size > 0)
    status = this->E_CONTINUE_BODY;
  else if (_size < 0)
    status = this->E_ERROR;
  else
    status = this->E_END_REQUEST;
  return (status);
}

void	HttpBuffer::clear()
{
  if (this->_transaction != NULL)
    {
      delete _transaction;
      _transaction = NULL;
    }
  _header.clear();
  _body.clear();
  _step = this->E_HEADER;
  if (_ast != NULL)
  {
    _ast->remove();
    delete _ast;
    this->_ast = NULL;
  }
  _size = 0;
  _hsize = 0;
}

ApiImpl::Transaction *HttpBuffer::getTransaction() const
{
	return (this->_transaction);
}

void HttpBuffer::setClientSocket(zia::api::network::IEndPoint *cep)
{
	this->_clientEndP = cep;
}

void HttpBuffer::setServerSocket(zia::api::network::IEndPoint *sep)
{
	this->_clientEndP = sep;
}

const std::string &HttpBuffer::getHeader() const
{
	return (this->_header);
}

std::stringstream &HttpBuffer::getBody()
{
	return (this->_body);
}
