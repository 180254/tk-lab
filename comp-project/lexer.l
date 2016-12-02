%option noyywrap
%option yylineno

/* -------------------------------------------------------------------------------------------- */

LETTER          [a-zA-Z]
DIGIT           [0-9]
ID              {LETTER}({LETTER}|{DIGIT})*

DIGITS          {DIGIT}{DIGIT}*
OPT_FRACTION    (\.{DIGITS})?
OPT_EXPONENT    ([eE][-+]?{DIGITS})?
NUM             {DIGITS}{OPT_FRACTION}{OPT_EXPONENT}

/* -------------------------------------------------------------------------------------------- */

%{

#include "global.h"

%}

/* -------------------------------------------------------------------------------------------- */

%%

\{[^}]*\} { /* comments */ }
[ \t\n] { /* whitespaces */ }

"program"       return L_PROGRAM;
"var"           return L_VAR;
"array"         return L_ARRAY;
".."            return L_ARRAY_RANGE;
"of"            return L_OF;
"integer"       return L_INTEGER;
"real"          return L_REAL;
"function"      return L_FUNCTION;
"procedure"     return L_PROCEDURE;
"begin"         return L_BEGIN;
"end"           return L_END;
"if"            return L_IF;
"then"          return L_THEN;
"else"          return L_ELSE;
"while"         return L_WHILE;
"do"            return L_DO;
"not"           return L_NOT;
"div"           return L_DIV;
"mod"           return L_MOD;
"and"           return L_AND;
"or"            return L_OR;

":="            return ASSIGN;

"="             return EQ;
"<>"            return NE;
"<="            return LE;
">="            return GE;
"<"             return LO;
">"             return GR;

{ID}            return ID;
{NUM}           return NUM;

.               return *yytext;
                    
%%

/* -------------------------------------------------------------------------------------------- */


