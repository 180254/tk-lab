#pragma once

#include "parser.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// lexer
int yylex();
extern int yylineno;
extern FILE *yyin;

// error
void yyerror(const char *); // error.cpp



