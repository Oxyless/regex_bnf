#ifndef __LEXER_BNF__
#define __LEXER_BNF__

/* 
Lexer universel version 2.0 - certifié non portable linux 

Utilisation (en supposant le que le fichier de bnf se nomme HttpBnf.txt):

Méthode 1

#include <iostream>
#include "Lexer.h"

int main()
{
Bnf::Lexer lex;
bool rtn_parseur;
bool rtn_lexer;

lex.setSeparators(" \n\t");
lex.setSpecials("()[]#*!~|$:=>");
rtn_lexer = lex.run("HttpBnf.txt");
}

Comment savoir si le lexer a réussi ? c'est le retour de lexer.run(...) (type bool)
Comment afficher la liste de token ? lexer.displayList()
Comment afficher la bnfMap ? lexer.displayMap()
Comment réinitialiser le lexer ? un appel lexer.reinit()

Les separateurs sont aussi appellés Blanks sur un codeworker
Les specials sont des caracteres qui seront considérés comme token (de 1 char)
---
Clement Bruchon -- 24 Avril 2010 -- brucho_c@epitech.net
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

namespace Bnf
{
	class Lexer
	{
		enum e_StatusEnum
		{
			UNKNOWN,
			TOKEN,
			SEPARATOR,
			DOUBLE_QUOTE,
			QUOTE,
			SPECIAL,
			COMMENT
		};

	public:
		Lexer(void);
		~Lexer(void);

		bool													run(const char *fileName);
		bool													createTokenList();
		bool													createBnfMap(void);
		const std::string&										getSeparators(void);
		const std::list<std::string>&							getTokenList();
		const std::map<std::string, std::list<std::string> >&	getTokenMap();
		void													setSeparators(const std::string &);
		void													addSeparators(const std::string &);
		void													setSpecials(const std::string &);
		void													addSpecial(const std::string &);
		void													setSaveSeparators(bool);
		void													displayList();
		void													displayMap();
		bool													openStream(const char *fileName);
		bool													isMapEmpty();
		bool													isListEmpty();
		void													clearList();
		void													clearMap();
		void													reinit();

	private:
		bool													_isSeparator(const char);
		bool													_isSpecial(const char);
		void													_getStatus(const char);
		void													_getSeparator(const char);
		void													_getToken(const char);
		void													_getDoubleQuote(const char);
		void													_getQuote(const char);
		void													_getUnknown(const char);
		void													_getSpecial(const char);
		void													_getBackslash(const char);
		void													_getComment(const char);
		void													_saveToken();

	private:
		unsigned int											_backSlash;
		bool													_listEmpty;
		bool													_mapEmpty;
		std::string												_separators;
		std::string												_specials;
		bool													_saveSeparators;
		std::ifstream											_stream;
		e_StatusEnum											_status;
		std::string												_token;
		std::string												_num;
		bool													_numFlag;
		std::list<std::string>									_tokenList;
		std::map<std::string, std::list<std::string> >			_tokenMap;

		const unsigned int										BLOCK_SIZE;
	};
}

#endif
