#include "global.hpp"

void yyerror(const char* message)
{
    cerr << "lex error, line=" << yylineno << ": " << message << "\n";
}
