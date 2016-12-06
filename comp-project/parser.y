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
    void*                 none;
    string*               str;
    vector<string*>*      v_str;
    Type*                 type;
    Symbol*               symbol;
    vector<Symbol*>*      v_symbol;
    Operation             oper;
    Expression*           expr;
    vector<Expression*>*  v_expr;
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
%type <v_expr>   compound_statement
%type <v_expr>   optional_statements
%type <v_expr>   statement_list
%type <v_expr>   statement
%type <expr>     variable
%type <expr>     procedure_statement
%type <v_expr>   expression_list
%type <expr>     expression
%type <expr>     simple_expression
%type <expr>     term
%type <expr>     factor
%type <oper>     relop
%type <oper>     sign
%type <oper>     mulop
%type <oper>     or
%type <oper>     assignop
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
    subprogram_declarations {
    }
    compound_statement {
        program = *$12;
    }
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

compound_statement : // vector<Expression*>*
    T_BEGIN
    optional_statements
    T_END {
        $$ = $2;
    }
    ;
    
optional_statements : // vector<Expression*>*
    statement_list {
        $$ = $1;
    }
    | %empty {
        $$ = new vector<Expression*>();
    }
    ;

statement_list : // vector<Expression*>*
    statement {
        $$ = new vector<Expression*>();
        
        for(auto st : *$1) {
            $$->push_back(st);
        }
        
        DELETE($1);
    }
    | statement_list ';' statement {
        $$ = new vector<Expression*>();
        
        for(auto st : *$1) {
            $$->push_back(st);
        }
        for(auto st : *$3) {
            $$->push_back(st);
        }
          
        DELETE($1);
        DELETE($3);
    } 
    ;
    
statement : // vector<Expression*>*
    variable assignop expression {
        auto expr = new Expression();
        expr->oper = OP_ASSIGN;
        expr->args->push_back(expr_arg_expr($1));
        expr->args->push_back(expr_arg_expr($3));
        expr->line = yylineno;
        
        $$ = new vector<Expression*>();
        $$->push_back(expr);
    }
    | procedure_statement {
        $$ = new vector<Expression*>();
        $$->push_back($1);

    }
    | compound_statement {
        $$ = $1;
    }
    | T_IF expression T_THEN statement {
        auto expr = new Expression();
        expr->oper = OP_FLOW_IF;
        expr->args->push_back(expr_arg_expr($2));
        expr->args->push_back(expr_arg_expr_v($4));
        expr->line = yylineno;
        
        $$ = new vector<Expression*>();
        $$->push_back(expr);
    }
    | T_IF expression T_THEN statement T_ELSE statement {
        auto expr = new Expression();
        expr->oper = OP_FLOW_IF_THEN;
        expr->args->push_back(expr_arg_expr($2));
        expr->args->push_back(expr_arg_expr_v($4));
        expr->args->push_back(expr_arg_expr_v($6));
        expr->line = yylineno;
        
        $$ = new vector<Expression*>();
        $$->push_back(expr);
    }
    | T_WHILE expression T_DO statement {
        auto expr = new Expression();
        expr->oper = OP_FLOW_WHILE;
        expr->args->push_back(expr_arg_expr($2));
        expr->args->push_back(expr_arg_expr_v($4));
        expr->line = yylineno;
        
        $$ = new vector<Expression*>();
        $$->push_back(expr);
    } 
    ;
    
variable : // Expression*
    id {
        $$ = new Expression();
        $$->oper = OP_ID;
        $$->args->push_back(expr_arg_id($1));
        $$->line = yylineno;
    }
    | id '[' expression ']' {
        $$ = new Expression();
        $$->oper = OP_ARRAY_ACCESS;
        $$->args->push_back(expr_arg_id($1));
        $$->args->push_back(expr_arg_expr($3));
        $$->line = yylineno;
    }
    ;

procedure_statement : // Expression*
    id { // ???
        $$ = new Expression();
        $$->oper = OP_CALL_FUNC;
        $$->args->push_back(expr_arg_id($1));
        $$->line = yylineno;
    }
    | id '(' expression_list ')' {
        $$ = new Expression();
        $$->oper = OP_CALL_FUNC;
        $$->args->push_back(expr_arg_id($1));
        
        for(auto expr : *$3) {
            $$->args->push_back(expr_arg_expr(expr));
        }
        
        $$->line = yylineno;
        
        DELETE($3);
    }
    ;
    
expression_list: // vector<Expression*>*
    expression {
        $$ = new vector<Expression*>();
        $$->push_back($1);
    }
    | expression_list ',' expression {
        $$ = $1;
        $1->push_back($3);
    }
    ;
    
expression : // Expression*
    simple_expression {
        $$ = $1;
        $$->line = yylineno;
    }
    | simple_expression relop simple_expression {
        $$ = new Expression();
        $$->oper = $2;
        $$->args->push_back(expr_arg_expr($1));
        $$->args->push_back(expr_arg_expr($3));
        $$->line = yylineno;
    }
    ;
    
simple_expression : // Expression*
    term {
        $$ = $1;
        $$->line = yylineno;
    }
    | sign term %prec UMINUS {
        $$ = new Expression();
        $$->oper = $1 == OP_MATH_MINUS ? OP_MATH_UMINUS : OP_MATH_UPLUS;
        $$->args->push_back(expr_arg_expr($2));
        $$->line = yylineno;
    }
    | simple_expression sign term {
        $$ = new Expression();
        $$->oper = $2;
        $$->args->push_back(expr_arg_expr($1));
        $$->args->push_back(expr_arg_expr($3));
        $$->line = yylineno;
    }
    | simple_expression or term {
        $$ = new Expression();
        $$->oper = $2;
        $$->args->push_back(expr_arg_expr($1));
        $$->args->push_back(expr_arg_expr($3));
        $$->line = yylineno;
    }
    ;
    
term : // Expression*
    factor {
        $$ = $1;
    }
    | term mulop factor {
        $$ = new Expression();
        $$->oper = $2;
        $$->args->push_back(expr_arg_expr($1));
        $$->args->push_back(expr_arg_expr($3));
        $$->line = yylineno;
    }
    ;
    
factor : // Expression*
    variable {
        $$ = $1;
    }
    | id '(' expression_list ')' {
        $$ = new Expression();
        $$->oper = OP_CALL_FUNC;
        $$->args->push_back(expr_arg_id($1));
        
        for(auto expr : *$3) {
            $$->args->push_back(expr_arg_expr(expr));
        }
        
        $$->line = yylineno;
        
        DELETE($3);
    }
    | num {
        $$ = new Expression();
        $$->oper = OP_CONSTANT;
        $$->args->push_back(expr_arg_const($1));
        $$->line = yylineno;
        
    }
    | '(' expression ')' {
        $$ = $2;
    }
    | T_NOT factor {
        $$ = new Expression();
        $$->oper = OP_LOG_NOT;
        $$->args->push_back(expr_arg_expr($2)); 
        $$->line = yylineno;
    }
    ;         

/* -------------------------------------------------------------------------------------------- */

relop : // Operation
    T_EQ { $$ = OP_LOG_EQ; }
    | T_NE { $$ = OP_LOG_NE; }
    | T_LE { $$ = OP_LOG_LE; } 
    | T_GE { $$ = OP_LOG_GE; }
    | T_LO  { $$ = OP_LOG_LO; }
    | T_GR { $$ = OP_LOG_GR; }
    ;
    
sign : // Operation
    '-' { $$ = OP_MATH_MINUS; }
    | '+' { $$ = OP_MATH_PLUS; }
    ;
    
mulop : // Operation
    '*' { $$ = OP_MATH_MUL; }
    | '/' { $$ = OP_MATH_DIV1; }
    | T_DIV { $$ = OP_MATH_DIV2; }
    | T_MOD { $$ = OP_MATH_MOD; }
    | T_AND { $$ = OP_LOG_AND; }
    ;  
       
or : // Operation
    T_OR { $$ = OP_LOG_OR; }
    ;

assignop : // Operation
    T_ASSIGN { $$ = OP_ASSIGN; }
    ;
    
num : // string*
    T_NUM { $$ = yylval.str; }
    ;
    
id  : // string*
    T_ID { $$ = yylval.str; }
    ;
   
%%

/* -------------------------------------------------------------------------------------------- */



