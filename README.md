regex_bnf
=========

A homemade regex engine based on e-bnf syntax (example available with http bnf).

```
/* 
Parseur de bnf version 2.0

Liste des symboles gérés : 
---------------------------------------------------------------------------------
...:=... -> implémente une règle demandant au moins un espace entre chaque token
...:==...-> implémente une règle demandantaucun espace entre chaque token (les espaces sont cependant skippés en entrant dans la regle)
...:=>...-> implémente une règle ignorqnt les espaces entre chaque token
"..."        -> chaîne requise literalement (important ! "" correspond à n'importe quel caractère)
'...'        -> chaîne requise literalement (important ! '' correspond à n'importe quel caractère)
token        -> regle requise
[...]        -> bloque optionnel
(...)        -> bloque obligatoire
~"..."        -> caracteres acceptés en boucle par "liste blanche" 
!"..."        -> caracteres acceptés en boucle par "liste noire" (Tips ! !"" consomme tout le flux)
n ...        -> bloque requis n fois**        ** sauf pour *$~!
n* ...  -> necessite au moins n passage pour reussir
n$ ...  -> necessite au moins n passage pour reussir (les virgules ne comptes pas comme un passage)
n~"..." -> necessite un token de n characteres pour reussir
n!"..." -> necessite un token de n characteres pour reussir
*...        -> bloque accepté un nombre indéterminé de fois 
#...        -> bloque accepté un nombre indéterminé de fois séparé par des virgules 
...|...        -> "ou" logique
;....\n        -> commentaires
$"..."        -> lit le flux jusqu'au mot entre double quote (s'arrete juste avant le mot)
---------------------------------------------------------------------------------

Important ! En cas d'erreur dans la bnf : le preparseur ou le lexer genereront une Bnf::BnfException
Important ! la classe Parser est thread-safe
Important ! dans le retour de parser.run(), l'ast est "new", donc ne pas oublier de le free apres utilisation

Tips ! Si le parseur echoue, il renvoi null
Tips ! pour etre sur que les regles avec "n" (cf plus haut) fonctionnent correctement, ne pas hesiter a mettre un espace apres le nombre
Tips ! Bnf::Tree implemente une methode display() (pratique pour savoir quoi prendre avec parse ast)
Tips ! l'objet ParseAst implemente 3 manieres de recuperer les information dans l'ast (par whiteList, par blackList, par noeud)
Tips ! le define TAKENODE permet de prendre tout a partir d'un noeud
Tips ! si le parseur ne reagit pas comme tu veux, relis ta bnf

Utilisation (en supposant le que le fichier de bnf se nomme HttpBnf.txt et que l'on veut verifier la regle Request):

#include <iostream>
#include <string>
#include "Lexer.h"
#include "PreParser.h"
#include "Parser.h"
#include "Tree.h"
#include "ParseAst.h"

int main()
{
Bnf::Lexer lex;
lex.setSeparators(" \r\n\t");
lex.run("HttpBnf.txt");

Bnf::PreParser PreParser;
PreParser.run(lex.getTokenMap(), "Request");

Bnf::Parser parser;
Bnf::Tree *ast = parser.run(PreParser, "Request", "POST /actions/user_login.php?return_to= HTTP/1.1\r\nContent-Length : 42\r\n\r\n");

if (ast == NULL)
std::cout << "error" << std::endl;

Bnf::ParseAst parseast;

const char *needed[] = {"Request-Line",  "Method", TAKENODE,  NULL};

std::string method = parseast.getByWhiteList(*ast, needed);
std::cout << method << std::endl;

return (0);
}

-> vitesse de traitement : 12 000 requetes http/seconde.

*/
```
