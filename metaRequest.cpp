#include "metaRequest.h"

Bnf::metaRequest::metaRequest(const std::string &req) : data(req)
{
	this->pos = 0;
	this->fin = req.size();
	this->separators = " \t";
}