#include "global.hpp"

void yyerror(const char* message) 
{
    cerr << "lex error: " << message << " (line: " << yylineno << ")" << "\n";
}
