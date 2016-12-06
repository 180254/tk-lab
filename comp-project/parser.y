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
    void*             none;
    string*           str;
    vector<string*>*  v_str;
    Type*             type;
    Symbol*           symbol;
    vector<Symbol*>*  v_symbol;
}

// %type <none>   program
%type <v_str>     identifier_list
%type <v_symbol>  declarations
%type <type>      type
%type <type>      standard_type
// %type <none>   subprogram_declarations
// %type <none>   subprogram_declaration
// %type <none>   subprogram_head
// %type <none>   arguments
// %type <none>   parameter_list
// %type <none>   compound_statement
// %type <none>   optional_statements
// %type <none>   statement_list
// %type <none>   statement
// %type <none>   variable
// %type <none>   procedure_statement
// %type <none>   expression_list
// %type <none>   expression
// %type <none>   simple_expression
// %type <none>   term
// %type <none>   factor
// %type <none>   relop
// %type <none>   sign
// %type <none>   mulop
// %type <none>   or
// %type <none>   assignop
%type <str>      num
%type <str>      id

/* -------------------------------------------------------------------------------------------- */

%%

/* -------------------------------------------------------------------------------------------- */

program :
    T_PROGRAM id '(' identifier_list ')' ';' {
        DELETE($2);
        DELETE($4);
    }
    declarations {
        for (auto symbol : *$8) {
            memory.push_back(symbol);
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
    
declarations : // vector<Symbol*>*
    declarations T_VAR identifier_list ':' type ';' {
        $$ = new vector<Symbol*>();

        for (auto symbol : *$1) {
            $$->push_back(symbol);  
        }
        DELETE($1);
        
        for (auto ident : *$3) {
            Symbol* symbol = new Symbol();
            symbol->type = new Type(*$5);
            symbol->name = ident;
            $$->push_back(symbol);
        }
        
        DELETE($3);
        DELETE($5);
    }
    | %empty {
        $$ = new vector<Symbol*>();
    }
    ;

type : // Type*
    standard_type {
        $$ = $1;
    }
    | T_ARRAY '[' num T_ARRAY_RANGE num ']' T_OF standard_type {
        $$ = new Type();
        $$->te = TE_ARRAY;
        Array* array = new Array();
        array->te = $8->te;
        array->min = stoi(*$3);
        array->max = stoi(*$5);
        $$->array = array;
        
        DELETE($8);
    }
    ;
    
standard_type : // Type*
    T_INTEGER {
        $$ = new Type();
        $$->te = TE_INTEGER;
    }
    | T_REAL {
        $$ = new Type();
        $$->te = TE_REAL;
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

parameter_list :
    identifier_list ':' type
    | parameter_list ';' identifier_list ':' type
    ;

compound_statement :
    T_BEGIN
    optional_statements
    T_END
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



