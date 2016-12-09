#include "global.hpp"

void yyerror(const char* message) {
    cerr << "lex error, line=" << yylineno << ": " << message << "\n";
}

void sem_error(int line, const char* message) {
    cerr << "sem err, line=" << line << ": "<< message << "\n";
}
