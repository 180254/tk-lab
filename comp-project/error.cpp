#include "global.h"

void yyerror(const char *message) 
{
    fprintf(stderr, "yyerror: line %d, %s\n", yylineno, message);
}

