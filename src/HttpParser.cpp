#include "bnf/HttpParser.h"

httpParser::httpParser()
{
    _lex.setSeparators(" \r\n\t");
    _lex.setSpecials("()[]#*!~|$:=>");
    _lex.run("HttpBnf.txt");
    _preParser.run(_lex.getTokenMap(), "Request");
    _preParserSSL.run(_lex.getTokenMap(), "SSL_REQ");
}

Bnf::Tree *httpParser::parse(const std::string &rule, const std::string &request)
{
    return (this->run(this->_preParser, rule, request));
}

Bnf::Tree *httpParser::parseSSL(const std::string &rule, const std::string &request)
{
    return (this->run(this->_preParserSSL, rule, request));
}