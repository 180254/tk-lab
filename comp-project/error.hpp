#pragma once

extern vector<string*> errors;

void yyerror(const char*);
void sem_error(int,const char*);
