%{

#define YYERROR_VERBOSE

#include "global.hpp"

%}

/* -------------------------------------------------------------------------------------------- */

%token T_PROGRAM
%token T_VAR
%token T_ARRAY
%token T_ARRAY_RANGE
%token T_OF
%token T_INTEGER
%token T_REAL
%token T_FUNCTION
%token T_PROCEDURE
%token T_BEGIN
%token T_END
%token T_IF
%token T_THEN
%token T_ELSE
%token T_WHILE
%token T_DO
%token T_NOT
%token T_DIV
%token T_MOD
%token T_AND
%token T_OR

%token T_ID
%token T_NUM

%token T_EQ
%token T_NE
%token T_LE
%token T_GE
%token T_LO
%token T_GR

%token T_ASSIGN

/* -------------------------------------------------------------------------------------------- */

%token UMINUS

%precedence T_THEN
%precedence T_ELSE

/* -------------------------------------------------------------------------------------------- */

%union {
    string          *str;
    vector<string*> *v_str;
    Type            type;
    Mem             *mem;
    vector<Mem*>    *v_mem;
}

%type <v_str>  identifier_list
%type <v_mem>  declarations
%type <mem>    type
%type <type>   standard_type
%type <str>    num
%type <str>    id

/* -------------------------------------------------------------------------------------------- */

%%

/* -------------------------------------------------------------------------------------------- */

program :
    T_PROGRAM id '(' identifier_list ')' ';' {
        DELETE($2);
        DELETE($4);
    }
    declarations {
        for (auto mem : *$8) {
            memory.push_back(mem);
        }
        DELETE($8);
    }
    subprogram_declarations
    compound_statement
    '.'
    ;
    
identifier_list : // vector<string*>*
    id {
        $$ = new vector<string*>();
        $$->push_back($1);
    }
    | identifier_list ',' id {
        $$->push_back($3);
    }
    ;
    
declarations : // vector<mem*>*
    declarations T_VAR identifier_list ':' type ';' {
        $$ = new vector<Mem*>();

        for (auto mem : *$1) {
            $$->push_back(mem);  
        }
        DELETE($1);
        
        for (auto ident : *$3) {
            Mem *mem = new Mem(*$5);
            mem->name = ident;
            $$->push_back(mem);
        }
        DELETE($3);
        DELETE($5);
    }
    | %empty {
        $$ = new vector<Mem*>();
    }
    ;

type : // Mem*
    standard_type {
        $$ = new Mem();
        $$->type = $1;
    }
    | T_ARRAY '[' num T_ARRAY_RANGE num ']' T_OF standard_type {
        $$ = new Mem();
        $$->array = new Array();
        $$->array->min = stoi(*$3);
        $$->array->max = stoi(*$5);
        $$->array->type = $8;
    }
    ;
    
standard_type : // Type
    T_INTEGER {
        $$ = INTEGER;
    }
    | T_REAL {
        $$ = REAL;
    }
    ;
    
subprogram_declarations :
    subprogram_declarations subprogram_declaration ';'
    | %empty;
    ;
    
subprogram_declaration :
    subprogram_head declarations compound_statement
    ;
    
subprogram_head :
    T_FUNCTION id arguments ':' standard_type ';'
    | T_PROCEDURE id arguments ';'
    ;

arguments :
    '(' parameter_list ')'
    | %empty;

parameter_list :
    identifier_list ':' type
    | parameter_list ';' identifier_list ':' type
    ;

compound_statement :
    T_BEGIN
    optionaT_statements
    T_END
    ;
    
optionaT_statements :
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
    | T_IF expression T_THEN statement
    | T_IF expression T_THEN statement T_ELSE statement
    | T_WHILE expression T_DO statement
    ;
    
variable :
    id
    | id '[' expression ']'
    ;

procedure_statement :
    id
    | id '(' expression_list ')'
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
    | sign term %prec UMINUS
    | simple_expression sign term
    | simple_expression or term
    ;
    
term :
    factor
    | term mulop factor
    ;
    
factor :
    variable
    | id '(' expression_list ')'
    | num
    | '(' expression ')'
    | T_NOT factor
    ;         

/* -------------------------------------------------------------------------------------------- */

relop :
    T_EQ | T_NE | T_LE | T_GE | T_LO  | T_GR
    ;
    
sign :
    '-' | '+'
    ;
    
mulop :
    '*' | '/' | T_DIV | T_MOD | T_AND
    ;  
       
or :
    T_OR
    ;

assignop :
    T_ASSIGN
    ;
    
num : // string*
    T_NUM {
        $$ = yylval.str;
    }
    ;
    
id  : // string*
    T_ID {
        $$ = yylval.str;
    }
    ;
   
%%

/* -------------------------------------------------------------------------------------------- */



