%{

#define YYERROR_VERBOSE

#include "global.h"

%}

/* ---------------------------------------------------------------------------------------------------------------- */

%token L_PROGRAM
%token L_VAR
%token L_ARRAY
%token L_ARRAY_RANGE
%token L_OF
%token L_INTEGER
%token L_REAL
%token L_FUNCTION
%token L_PROCEDURE
%token L_BEGIN
%token L_END
%token L_IF
%token L_THEN
%token L_ELSE
%token L_WHILE
%token L_DO
%token L_NOT
%token L_DIV
%token L_MOD
%token L_AND
%token L_OR

%token ID
%token NUM

%token EQ
%token NE
%token LO
%token LE
%token GR
%token GE

%token ASSIGN

/* ---------------------------------------------------------------------------------------------------------------- */

%%

/* ---------------------------------------------------------------------------------------------------------------- */

program :
    L_PROGRAM ID '(' identifier_list ')' ';'
    declarations
    subprogram_declarations
    compound_statement
    '.'
    ;
    
identifier_list :
    ID
    | identifier_list ',' ID
    ;
    
declarations :
    declarations L_VAR identifier_list ':' type ';'
    | %empty;

type :
    standard_type
    | L_ARRAY '[' NUM L_ARRAY_RANGE NUM ']' L_OF standard_type
    ;
    
standard_type :
    L_INTEGER
    | L_REAL
    ;
    
subprogram_declarations :
    subprogram_declarations subprogram_declaration ';'
    | %empty;
    ;
    
subprogram_declaration :
    subprogram_head declarations compound_statement
    ;
    
subprogram_head :
    L_FUNCTION ID arguments ':' standard_type ';'
    | L_PROCEDURE ID arguments ';'
    ;

arguments :
    '(' parameter_list ')'
    | %empty;

parameter_list :
    identifier_list ':' type
    | parameter_list ';' identifier_list ':' type
    ;

compound_statement :
    L_BEGIN
    optional_statements
    L_END
    ;
    
optional_statements :
    statement_list
    | %empty
    ;

statement_list :
    statement
    | statement_list ';' statement
    ;
    
statement :
    variable assignop expression
    | procedure_statement
    | compound_statement
    | L_IF expression L_THEN statement L_ELSE statement
    | L_WHILE expression L_DO statement
    ;
    
variable :
    ID
    | ID '[' expression ']'
    ;

procedure_statement :
    ID
    | ID '(' expression_list ')'
    ;
    
expression_list:
    expression
    | expression_list ',' expression
    ;
    
expression :
    simple_expression
    | simple_expression relop simple_expression
    ;
    
simple_expression :
    term
    | sign term
    | simple_expression sign term
    | simple_expression or term
    ;
    
term :
    factor
    | term mulop factor
    ;
    
factor :
    variable
    | ID '(' expression_list ')'
    | NUM
    | '(' expression ')'
    | L_NOT factor
    ;         

/* ---------------------------------------------------------------------------------------------------------------- */

relop :
    EQ 
    | NE 
    | LO
    | LE 
    | GR 
    | GE
    ;
    
sign :
    '-' 
    | '+'
    ;
    
mulop :
    '*'
    | '/'
    | L_DIV
    | L_MOD
    | L_AND
    ;  
       
or :
    L_OR
    ;

assignop:
    ASSIGN
    ;
   
%%

/* ---------------------------------------------------------------------------------------------------------------- */



