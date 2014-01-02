#ifndef __HTTP_PARSER_H__
#define __HTTP_PARSER_H__

#include "bnf/Lexer.h"
#include "bnf/Parser.h"
#include "bnf/PreParser.h"
#include "bnf/Tree.h"
#include "Singleton.hpp"

class httpParser : private Bnf::Parser, public Singleton<httpParser>
{
public:
    httpParser();
public:
    Bnf::Tree *parse(const std::string &rule, const std::string &request);
    Bnf::Tree *parseSSL(const std::string &rule, const std::string &request);

private:
    Bnf::PreParser _preParser;
    Bnf::PreParser _preParserSSL;
    Bnf::Lexer      _lex;
};

#endif