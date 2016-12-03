#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

// lexer
int yylex();
extern int yylineno;
extern FILE *yyin;

// error
void yyerror(const char *);

// parser
#include "parser.h"

