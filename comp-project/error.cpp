#include "global.hpp"

vector<string*> errors;

void yyerror(const char* message) {
    stringstream ss;
    ss << "lex error, line=" << yylineno << ": " << message;
    errors.push_back(new string(ss.str()));
}

void sem_error(int line, const char* message) {
    stringstream ss;
    ss << "sem err, line=" << line << ": "<< message;
    errors.push_back(new string(ss.str()));
}
