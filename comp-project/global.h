#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>

// node
#include "node.h"

// lexer
int yylex();
extern int yylineno;
extern FILE *yyin;

// error
void yyerror(const char *);

// parser
#include "parser.h"
