#pragma once

/* ---------------------------------------------------------------------------------------------*/

enum VarType {
    UNKNOWN,
    REAL,
    INTEGER,
    ARRAY    
};

struct Array {
    int min;
    int max;
    VarType var_type;
    
    Array();
    Array(const Array &obj);
    ~Array();
    
    std::string to_string();
};

struct Mem {
    std::string *name;
    VarType var_type;
    Array *array;
    int address;
    
    Mem();
    Mem(const Mem &obj);
    ~Mem();
    
    std::string to_string();
};

/* ---------------------------------------------------------------------------------------------*/

extern int mem_ptr;
extern std::vector<Mem*> memory; 

