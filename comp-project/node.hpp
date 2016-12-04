#pragma once

/* ---------------------------------------------------------------------------------------------*/

enum Type {
    UNKNOWN = 0,
    REAL    = 1,
    INTEGER = 2,
    ARRAY   = 3
};

struct Array {
    int  min;
    int  max;
    Type type;
    
    Array();
    Array(const Array &obj);
    ~Array();
    
    std::string str();
};

struct Mem {
    std::string *name;
    Type        type;
    Array       *array;
    int         address;
    
    Mem();
    Mem(const Mem &obj);
    ~Mem();
    
    std::string str();
};

/* ---------------------------------------------------------------------------------------------*/

extern std::vector<Mem *> memory;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug();
void mem_free();
