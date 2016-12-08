#pragma once

using namespace std;

/* ------------------------------------------------------------------------- */

enum TypeEnum : int;
struct Type;
struct Array;
struct Symbol;
enum Operation : int;
struct Expression;
struct ExprArg;
enum ExprArgType : int;
union ExprArgVal;
struct Function;
struct Memory;
typedef vector<Expression*> Program;

/* ------------------------------------------------------------------------- */

enum TypeEnum : int {
    TE_UNKNOWN = 0,
    TE_INTEGER = 1,
    TE_REAL    = 2,
    TE_ARRAY   = 3,
    TE_BOOLEAN = 4,
    TE_SPEC    = 5,
    TE_ERROR   = 6
};

/* ------------------------------------------------------------------------- */

struct Type {
    TypeEnum       te;
    Array*         array;
    bool           reference;
    
    Type();
    Type(const Type&);
    ~Type();
    
    bool operator==(const Type&);
    string str();
};

/* ------------------------------------------------------------------------- */

int type_size(Type*);
bool type_is_num(Type*);

/* ------------------------------------------------------------------------- */

struct Array {
    TypeEnum       te;
    int            min;
    int            max;
    
    Array();
    Array(const Array&);
    ~Array();
    
    bool operator==(const Array&);
    string str();
};

/* ------------------------------------------------------------------------- */

struct Symbol {
    string*        name;
    Type*          type;
    int            offset; 
    int            level;
    
    Symbol();
    Symbol(const Symbol&);
    ~Symbol();
    string str();
};

/* ------------------------------------------------------------------------- */

enum Operation : int {
    OP_UNKNOWN,
    OP_ID,
    OP_CONSTANT,
    OP_ASSIGN,
    OP_ARRAY_ACCESS,
    OP_MATH_PLUS,
    OP_MATH_MINUS,
    OP_MATH_UMINUS,
    OP_MATH_UPLUS,
    OP_MATH_MOD,
    OP_MATH_DIV1,
    OP_MATH_DIV2,
    OP_MATH_MUL,
    OP_FLOW_IF,
    OP_FLOW_IF_THEN,
    OP_FLOW_WHILE,
    OP_CALL_FUNC,
    OP_LOG_NOT,
    OP_LOG_AND,
    OP_LOG_OR,
    OP_LOG_NE,
    OP_LOG_LE,
    OP_LOG_GE,
    OP_LOG_LO,
    OP_LOG_GR,
    OP_LOG_EQ
};

/* ------------------------------------------------------------------------- */

struct Expression {
    Operation                oper;
    vector<ExprArg*>*        args;
    int                      result;
    int                      line;
    
    Expression();
    Expression(Operation);
    ~Expression();
    string str(int);
};

/* ------------------------------------------------------------------------- */

enum ExprArgType : int {
    E_UNKNOWN,
    E_ID_S,
    E_CONSTANT_S,
    E_EXPRESSION,
    E_PROGRAM
};

/* ------------------------------------------------------------------------- */

union ExprArgVal {
    int                   iVal;
    string*               sVal;
    Expression*           eVal;
    Program*              pVal;
};

/* ------------------------------------------------------------------------- */

struct ExprArg {
    ExprArgType    type;
    ExprArgVal     val;
    
    ExprArg();
    ~ExprArg();
    string str(int);
};

/* ------------------------------------------------------------------------- */

ExprArg* expr_arg_id(string*);
ExprArg* expr_arg_const(string*);
ExprArg* expr_arg_expr(Expression*);
ExprArg* expr_arg_prog(Program*);

/* ------------------------------------------------------------------------- */

struct Function {
    string*                name;
    vector<Symbol*>*       args;
    Type*                  result;
    Memory*                stack;
    Program*               body;
    
    Function();
    ~Function();
    string str(int);
};

/* ------------------------------------------------------------------------- */

struct Memory {
    vector<Symbol*>* vec;
    bool offset_asc;

    Memory();
    ~Memory();
};

/* ------------------------------------------------------------------------- */

extern Memory               memory;
extern vector<Function*>    functions;
extern Program              program;

/* ------------------------------------------------------------------------- */

int mem_find(Memory*, string);
int mem_add(Memory*, Symbol*, int);
int mem_temp(Memory*, TypeEnum);

/* ------------------------------------------------------------------------- */

void mem_debug();
void mem_free();
