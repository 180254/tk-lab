#pragma once

int yylex();
int yylex_destroy();
extern int yylineno;
extern FILE *yyin;
extern FILE *yyout;
