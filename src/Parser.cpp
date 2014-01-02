#include <string.h>
#include "Parser.h"

Bnf::Parser::Parser(void)
{
    this->_symbolParseFunctions.resize(Bnf::ENUM_SYMBOL_SIZE);

    this->_symbolParseFunctions[Bnf::UNKNOWN] = &Bnf::Parser::_symbolUnknown;
    this->_symbolParseFunctions[Bnf::RULE] = &Bnf::Parser::_parseRule;
    this->_symbolParseFunctions[Bnf::LITERAL] = &Bnf::Parser::_parseLiteral;
    this->_symbolParseFunctions[Bnf::OR] = &Bnf::Parser::_parseOr;
    this->_symbolParseFunctions[Bnf::WHITE_LIST] = &Bnf::Parser::_parseWhiteList;
    this->_symbolParseFunctions[Bnf::BLACK_LIST] = &Bnf::Parser::_parseBlackList;
    this->_symbolParseFunctions[Bnf::NOT_SKIP_BLANK] = &Bnf::Parser::_parseNotSkipBlanks;
    this->_symbolParseFunctions[Bnf::SKIP_BLANK] = &Bnf::Parser::_parseSkipBlanks;
    this->_symbolParseFunctions[Bnf::UNTIL] = &Bnf::Parser::_parseUntil;
    this->_symbolParseFunctions[Bnf::STAR] = &Bnf::Parser::_parseStar;
    this->_symbolParseFunctions[Bnf::BRACKET] = &Bnf::Parser::_parseBracket;
    this->_symbolParseFunctions[Bnf::PARENTHESIS] = &Bnf::Parser::_parseParenthesis;
    this->_symbolParseFunctions[Bnf::SHARP] = &Bnf::Parser::_parseSharp;	
    this->_symbolParseFunctions[Bnf::NUMBER] = &Bnf::Parser::_parseNumber;
}

Bnf::Parser::~Parser(void)
{
}

Bnf::Tree						*Bnf::Parser::run(PreParser &preParser, const std::string &rule, const std::string &request)
{
    Bnf::BnfTree				*bnfTree = NULL;
    Bnf::BnfOption              options(Bnf::BnfOption::CHECK_BLANCKS);

    if (!(bnfTree = _exist(preParser, rule)))
        throw (new BnfException("Bnf::Parser::run : the rule <", rule,"> doesn't exist."));

    Tree				*ast = new Tree(rule);	
    metaRequest			 mRequest(request);

    bool res = _resolve(*bnfTree, mRequest, *ast, options);
    _skipBlanks(mRequest);
    if (res == true)
        if (mRequest.pos != mRequest.fin)
            res = false;
    if (res == false)
    {
        ast->remove();
        if (ast != NULL)
          {
            delete ast;
            ast = NULL;
          }
        return (NULL);
    }
    return (ast);
}

bool							Bnf::Parser::_resolve(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption options)
{
    std::list<BnfTree *>::iterator it = bnfTree.begin();
    std::list<BnfTree *>::iterator ite = bnfTree.end();

    if (options.reinit == true)
        options.blanks = options.CHECK_BLANCKS;
    else
        options.reinit = true;

    while (it != ite)
    {
        if ((this->*this->_symbolParseFunctions[(*(*it)).getNodType()])(*(*it), mRequest, ast, options) == false)
            return (false);
        ++it;
    }

    return (true);
}

bool				Bnf::Parser::_parseRule(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    bool			rtn;

    ast.add(new Bnf::Tree(bnfTree.getNodName()));

    rtn = _resolve(bnfTree, mRequest, *(ast.top()), options);

    if (rtn == false)
        ast.remove();

    return (rtn);
}

bool                Bnf::Parser::_optionBlanks(Bnf::metaRequest &mRequest, Bnf::BnfOption &options)
{
    bool flag = false;

    if (options.blanks == options.NOT_SKIP_BLANK)
        return (true);
    while (mRequest.pos != mRequest.fin &&
        _readChar(mRequest.data[mRequest.pos], mRequest.separators))
    {
        ++mRequest.pos;
        flag = true;
    }
    if (options.blanks == options.SKIP_BLANK || mRequest.pos == 0)
        flag = true;

    return (flag);
}

bool                Bnf::Parser::_doOptions(Bnf::BnfTree &, Bnf::metaRequest &mRequest, Bnf::Tree &, Bnf::BnfOption &options)
{
    if (!_optionBlanks(mRequest, options))
        return (false);
    return (true);
}

bool				Bnf::Parser::_parseLiteral(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    const std::string&	word = bnfTree.getLiteral();
    unsigned int		pos = mRequest.pos;
    std::string			token;

    if (!_doOptions(bnfTree, mRequest, ast, options))
        return (false);

    for (unsigned int i = 0; i < word.size() ; ++i)
    {
        if (mRequest.pos == mRequest.fin)
            goto fail;
        if (_readChar(word[i], mRequest.data[mRequest.pos])) 
            token += word[i];
        else
            goto fail;
        ++mRequest.pos;
    }
    ast.add(new Tree(token));
    return (true);

fail:
    mRequest.pos = pos;
    return (false);
}


bool				Bnf::Parser::_parseOr(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    std::list<BnfTree *>::iterator it = bnfTree.begin();
    std::list<BnfTree *>::iterator ite = bnfTree.end();

    while (it != ite)
    {
        if ((this->*this->_symbolParseFunctions[(*(*it)).getNodType()])(*(*it), mRequest, ast, options) == true)
            return (true);
        ++it;
    }
    return (true);
}


bool				Bnf::Parser::_parseWhiteList(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    const std::string&	word = bnfTree.getLiteral();
    std::string         token;
    int                 nloop = -1;
    int                 cpt = 0;
    unsigned int		pos = mRequest.pos;

    if (!_doOptions(bnfTree, mRequest, ast, options))
        return (false);
    if (options.loop != -1)
    {
        nloop = options.loop;
        options.loop = -1;
    }

    while (mRequest.pos != mRequest.fin && (nloop == -1 || cpt < nloop))
    {
        if (_readChar(mRequest.data[mRequest.pos], word) == true)
            token += mRequest.data[mRequest.pos];
        else
            break ;
        ++mRequest.pos;
        ++cpt;
    }
    if (nloop != -1 && cpt < nloop)
    {
        mRequest.pos = pos;
        token = "";
    }
    if (token != "")
        ast.add(new Tree(token));
    return (!(token == ""));
}

bool				Bnf::Parser::_parseBlackList(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    const std::string&	word = bnfTree.getLiteral();
    std::string         token;
    int                 nloop = -1;
    int                 cpt = 0;
    unsigned int		pos = mRequest.pos;

    if (!_doOptions(bnfTree, mRequest, ast, options))
        return (false);
    if (options.loop != -1)
    {
        nloop = options.loop;
        options.loop = -1;
    }

    while (mRequest.pos != mRequest.fin && (nloop == -1 || cpt < nloop))
    {
        if (_readChar(mRequest.data[mRequest.pos], word) == false)
            token += mRequest.data[mRequest.pos];
        else
            break ;
        ++mRequest.pos;
        ++cpt;
    }
    if (nloop != -1 && cpt < nloop)
    {
        mRequest.pos = pos;
        token = "";
    }
    if (token != "")
        ast.add(new Tree(token));
    return (!(token == ""));
}

bool				Bnf::Parser::_parseNotSkipBlanks(Bnf::BnfTree &, Bnf::metaRequest &mRequest, Bnf::Tree &, Bnf::BnfOption &options)
{
    _skipBlanks(mRequest);
    options.blanks = options.NOT_SKIP_BLANK;
    return (true);
}

bool				Bnf::Parser::_parseSkipBlanks(Bnf::BnfTree &, Bnf::metaRequest &, Bnf::Tree &, Bnf::BnfOption &options)
{
    options.blanks = options.SKIP_BLANK;
    return (true);
}

bool				Bnf::Parser::_parseUntil(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &)
{
    std::string str = bnfTree.getLiteral();
    std::string token;
    unsigned int pos = mRequest.pos;
    bool dq = false;

    _skipBlanks(mRequest);
    if (mRequest.pos == mRequest.fin)
        return (false);
    while (mRequest.pos != mRequest.fin)
    {
        if (mRequest.data[mRequest.pos] == '"')
            dq = !dq;
        if (dq == true || strncmp(&mRequest.data[mRequest.pos], str.c_str(), str.size()))
        {
            token += mRequest.data[mRequest.pos];
            ++mRequest.pos;
        }
        else
        {
            ast.add(new Tree(token));
            return (true);
        }
    }
    mRequest.pos = pos;
    return (false);
}

bool				Bnf::Parser::_parseBracket(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    options.reinit = false;

    this->_resolve(bnfTree, mRequest, ast, options);
    return (true);
}

bool				Bnf::Parser::_parseParenthesis(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{
    options.reinit = false;

    return (this->_resolve(bnfTree, mRequest, ast, options));
}

bool				Bnf::Parser::_parseStar(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{  
    bool                 flag = true;
    int                 nloop = -1;
    int                 cpt = 0;
    unsigned int		pos = mRequest.pos;

    if (options.loop != -1)
    {
        nloop = options.loop;
        options.loop = -1;
    }

    while (mRequest.pos != mRequest.fin && flag == true)
    {
        options.reinit = false;
        flag = this->_resolve(bnfTree, mRequest, ast, options);
        if (flag)
            ++cpt;
    }

    if (nloop != -1 && cpt < nloop)
    {
        mRequest.pos = pos;
        while (cpt > 0)
        {
            bnfTree.remove();
            --cpt;
        }
        return (false);
    }
    return (true);
}

bool				Bnf::Parser::_parseSharp(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{  
    bool            flag = true;
    bool            firstTurn = true;
    int                 nloop = -1;
    int                 cpt = 0;
    unsigned int		pos = mRequest.pos;
    unsigned int		postmp;

    if (options.loop != -1)
    {
        nloop = options.loop;
        options.loop = -1;
    }

    while (mRequest.pos != mRequest.fin && flag == true)
    {
        options.reinit = false;
        flag = this->_resolve(bnfTree, mRequest, ast, options);

        if (flag == false)
        {
            if (firstTurn == false)
                --mRequest.pos;
            break ;
        }

        ++cpt;

        postmp = mRequest.pos;
        if (!_doOptions(bnfTree, mRequest, ast, options))
            break ;
        if (_readChar(',', mRequest.data[mRequest.pos])) 
            ++mRequest.pos;
        else
        {
            mRequest.pos = postmp;
            break ;
        }
        firstTurn = false;

    }
    if (nloop != -1 && cpt < nloop)
    {
        mRequest.pos = pos;
        while (cpt > 0)
        {
            bnfTree.remove();
            --cpt;
        }
        return (false);
    }
    return (true);
}

bool				Bnf::Parser::_parseNumber(Bnf::BnfTree &bnfTree, Bnf::metaRequest &mRequest, Bnf::Tree &ast, Bnf::BnfOption &options)
{ 
    unsigned int nb_loop = bnfTree.getNbLoop();
    unsigned int cpt = 0;
    unsigned int pos = mRequest.pos;
    Bnf::e_symbol symbol;

    symbol = (*(*bnfTree.begin())).getNodType();

    if (symbol == Bnf::BLACK_LIST || symbol == Bnf::WHITE_LIST ||
        symbol == Bnf::STAR || symbol == Bnf::SHARP)
    {
        options.loop = nb_loop;
        return ((this->*this->_symbolParseFunctions[symbol])((*(*bnfTree.begin())), mRequest, ast, options));
    }
    while (cpt < nb_loop)
    {
        if (this->_resolve(bnfTree, mRequest, ast, options) == false)
            goto fail;
        ++cpt;
    }
    if (cpt == nb_loop)
        return (true);
fail:
    pos = mRequest.pos;
    return (false);
}


Bnf::BnfTree *Bnf::Parser::_exist(PreParser &preParser, const std::string &ruleName)
{
    Bnf::PreParser::t_cacheMap::const_iterator it = preParser.getBnfCache().find(ruleName);

    if (it != preParser.getBnfCache().end())
        return ((*it).second);
    return (NULL);
}

bool				Bnf::Parser::_symbolUnknown(Bnf::BnfTree &bnfTree, Bnf::metaRequest &, Bnf::Tree &, Bnf::BnfOption &)
{
    char c = (bnfTree.getNodType() + '0');

    throw (new BnfException("Bnf::Parser::_symbolUnknown : the symbol <", &c ,"> doesn't exist."));

    return (false);
}
