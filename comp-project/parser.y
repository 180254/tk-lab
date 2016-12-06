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
// %type <none>   compound_statement
// %type <none>   optional_statements
// %type <none>   statement_list
// %type <none>   statement
%type <expr>     variable
%type <v_expr>   procedure_statement
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
    
variable : // Expression*
    id {
        $$ = new Expression();
        $$->oper = OP_ID;
        
        auto arg = new ExprArg();
        arg->type = E_ID_S;
        arg->val->sVal = new string(*$1);
        
        $$->line = yylineno;
        
        DELETE($1);
    }
    | id '[' expression ']' {
        $$ = new Expression();
        // TODO
    }
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
    
factor : // Expression*
    variable {
        $$ = $1;
    }
    | id '(' expression_list ')' {
        $$ = new Expression();
        $$->oper = OP_CALL_FUNC;
        
        auto name = new ExprArg();
        name->type = E_ID_S;
        name->val->sVal = new string(*$1);
        $$->args->push_back(name);
        
        for(auto expr : *$3) {
            auto arg = new ExprArg();
            arg->type = E_EXPRESSION;
            arg->val->eVal = expr;
            $$->args->push_back(arg);
        }
        
        DELETE($1);
        DELETE($3);
    }
    | num {
        $$ = new Expression();
        $$->oper = OP_CONSTANT;
        
        auto arg = new ExprArg();
        arg->type = E_CONSTANT_S;
        arg->val->sVal = $1;
        
        $$->args->push_back(arg);
        
    }
    | '(' expression ')' {
        $$ = $2;
    }
    | T_NOT factor {
        $$ = new Expression();
        $$->oper = OP_LOG_NOT;
        
    }
    ;         

/* -------------------------------------------------------------------------------------------- */

relop :
    T_EQ { return OP_LOG_EQ; }
    | T_NE { return OP_LOG_NE; }
    | T_LE { return OP_LOG_LE; } 
    | T_GE { return OP_LOG_GE; }
    | T_LO  { return OP_LOG_LO; }
    | T_GR {return OP_LOG_GR; }
    ;
    
sign :
    '-' { return OP_MATH_MINUS; }
    | '+' { return OP_MATH_PLUS; }
    ;
    
mulop :
    '*' { return OP_MATH_MUL; }
    | '/' { return OP_MATH_DIV1; }
    | T_DIV { return OP_MATH_DIV2; }
    | T_MOD { return OP_MATH_MOD; }
    | T_AND { return OP_LOG_AND; }
    ;  
       
or :
    T_OR { return OP_LOG_OR; }
    ;

assignop :
    T_ASSIGN { return OP_ASSIGN; }
    ;
    
num : // string*
    T_NUM { $$ = yylval.str; }
    ;
    
id  : // string*
    T_ID { $$ = yylval.str; }
    ;
   
%%

/* -------------------------------------------------------------------------------------------- */



