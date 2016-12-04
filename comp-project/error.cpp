#include "global.hpp"

void yyerror(const char *message) 
{
    cerr << "yyerror: line " << yylineno << ", " << message << "\n";
}

