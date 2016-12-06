#pragma once

/* ---------------------------------------------------------------------------------------------*/

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

/* ---------------------------------------------------------------------------------------------*/

enum TypeEnum : int {
    TE_UNKNOWN = 0,
    TE_INTEGER = 1,
    TE_REAL    = 2,
    TE_ARRAY   = 3,
    TE_BOOLEAN = 4,
    TE_ERROR   = 5
};

struct Type {
    TypeEnum       type;
    Array*         array;
    
    Type();
    Type(const Type &other);
    ~Type();
    
    bool operator==(const Type& other);
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

struct Array {
    TypeEnum       type;
    int            min;
    int            max;
    
    Array();
    Array(const Array &other);
    ~Array();
    
    bool operator==(const Array& other);
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

struct Symbol {
    std::string*   name;
    Type*          type;
    int            offset; 
    bool           reference;
    
    Symbol();
    ~Symbol();
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

enum Operation : int {
    OP_UNKNOWN,
    OP_ID,
    OP_ASSIGN,
    MATH_PLUS,
    MATH_MINUS,
    MATH_UMINUS,
    FLOW_IF,
    FLOW_WHILE,
    CALL_PROC,
    CALL_FUNC,
    LOG_NOT,
    LOG_AND,
    LOG_OR,
    LOG_NE,
    LOG_LE,
    LOG_GE,
    LOG_LO,
    LOG_GR,
    LOG_EQ
};

/* ---------------------------------------------------------------------------------------------*/

struct Expression {
    Operation                oper;
    std::vector<ExprArg*>*   args;
    int                      result; // index, -1 for error
    int                      line;
    
    Expression();
    ~Expression();
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

struct ExprArg {
    ExprArgType    type;
    ExprArgVal*    val;
    
    ExprArg();
    ~ExprArg();
    std::string str();
};

enum ExprArgType : int {
    E_UNKNOWN,
    E_ID,
    E_EXPRESSION
};

union ExprArgVal {
    int          iVal;
    Expression*  eVal;
};

/* ---------------------------------------------------------------------------------------------*/

struct Function {
    std::string*                name;
    std::vector<Type*>*         args;
    Type*                       result;
    std::vector<Symbol*>*       memory;
    std::vector<Expression*>*   expr;
};

/* ---------------------------------------------------------------------------------------------*/

extern std::vector<Symbol*>     memory;
extern std::vector<Function*>   functions;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug();
void mem_free();
