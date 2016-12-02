%option noyywrap

%{

#define YY_DECL int lexan (void)
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include "global.h"
#include <stdlib.h> /* atoi */

int lineno = 1;
int tokenval = NONE;

%}

%%

[ \t] {
	/* skip */
}

\n	{
	lineno++;
}

[a-zA-Z][a-zA-Z0-9]* {
	int p;
	
	p = lookup(yytext);
	if (p == 0)
		p = insert(yytext, ID);
		
	tokenval = p;
	return symtable[p].token;
}

[0-9]+	{
	tokenval = atoi(yytext);
	return NUM;
}

<<EOF>> {
	return DONE;
}

. {
	tokenval = NONE;
	return yytext[0];
}

%%

