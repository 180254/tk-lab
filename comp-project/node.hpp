#pragma once

/* ---------------------------------------------------------------------------------------------*/

// enum Type;
struct Symbol;
struct Array;
// enum Operation;
// enum ArgType;
union ArgVal;
struct Arg;
struct Expression;

/* ---------------------------------------------------------------------------------------------*/

enum Type {
    UNKNOWN = 0,
    INTEGER = 1,
    REAL    = 2,
    ARRAY   = 3,
    BOOLEAN = 4,
    ERROR   = 5
};

/* ---------------------------------------------------------------------------------------------*/

struct Symbol {
    std::string* name;
    Type         type;
    void*        info; // Array, ?
    int          offset; 
    bool         reference;
    
    Symbol();
    Symbol(const Symbol &obj);
    ~Symbol();
    
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

struct Array {
    Type type;
    int  min;
    int  max;
    
    Array();
    Array(const Array &obj);
    ~Array();
    
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

enum Operation {
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

enum ArgType {
    ID,
    EXPRESSION
};

union ArgVal {
    int          iVal;
    Expression*  eVal;
};

struct Arg {
    ArgType      type;
    ArgVal       val;
};

/* ---------------------------------------------------------------------------------------------*/

struct Expression {
    Operation                oper;
    std::vector<Arg>*        args;
    int                      result; // -1 for error
    int                      line;
};

/* ---------------------------------------------------------------------------------------------*/

extern std::vector<Symbol*> memory;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug();
void mem_free();
