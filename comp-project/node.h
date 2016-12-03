#pragma once

/* ---------------------------------------------------------------------------------------------*/

enum Type {
    UNKNOWN = 0,
    REAL = 1,
    INTEGER = 2,
    ARRAY = 3
};

struct Array {
    int min;
    int max;
    Type type;
    
    Array();
    Array(const Array &obj);
    ~Array();
    
    std::string to_string();
};

struct Mem {
    std::string *name_ptr;
    Type type;
    Array *array_ptr;
    int address;
    
    Mem();
    Mem(const Mem &obj);
    ~Mem();
    
    std::string to_string();
};

/* ---------------------------------------------------------------------------------------------*/

extern std::vector<Mem*> memory;
extern int memory_current;

