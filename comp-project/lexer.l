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

"program"       return T_PROGRAM;
"var"           return T_VAR;
"array"         return T_ARRAY;
".."            return T_ARRAY_RANGE;
"of"            return T_OF;
"integer"       return T_INTEGER;
"real"          return T_REAL;
"function"      return T_FUNCTION;
"procedure"     return T_PROCEDURE;
"begin"         return T_BEGIN;
"end"           return T_END;
"if"            return T_IF;
"then"          return T_THEN;
"else"          return T_ELSE;
"while"         return T_WHILE;
"do"            return T_DO;
"not"           return T_NOT;
"div"           return T_DIV;
"mod"           return T_MOD;
"and"           return T_AND;
"or"            return T_OR;

":="            return T_ASSIGN;

"="             return T_EQ;
"<>"            return T_NE;
"<="            return T_LE;
">="            return T_GE;
"<"             return T_LO;
">"             return T_GR;

{ID}            {
    yylval.str = new std::string(yytext);
    return T_ID;
}

{NUM}           {
    yylval.str = new std::string(yytext);
    return T_NUM;
}

.               return *yytext;
                    
%%

/* -------------------------------------------------------------------------------------------- */


