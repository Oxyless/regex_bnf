#ifndef __BNF_ENUM_H__
#define __BNF_ENUM_H__

namespace Bnf
{

	enum e_symbol
	{
		LITERAL,			//0
		RULE,				//1
		BRACKET,			//2
		PARENTHESIS,		//3
		STAR,				//4
		NUMBER,				//5
		SHARP,				//6
		WHITE_LIST,			//7
		BLACK_LIST,			//8
		OR,					//9
		UNTIL,				//10
		SKIP_BLANK,			//11
		NOT_SKIP_BLANK,		//12
		UNKNOWN,			//13
		ENUM_SYMBOL_SIZE	//14
	};

}

#endif