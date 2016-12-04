#include "global.hpp"

/* ---------------------------------------------------------------------------------------------*/

Array::Array() : min(-1), max(-1), type(UNKNOWN) {

}

Array::Array(const Array &obj) {
    min = obj.min;
    max = obj.max;
    type = obj.type;
}

Array::~Array() {

}

string Array::str() {
    stringstream ss;
    
    ss << "[";
    ss << "min: " << min << "; ";
    ss << "max: " << max << "; ";
    ss << "type: " << type << ";";
    ss << "]";
    
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

Mem::Mem() : name(nullptr), type(UNKNOWN), array(nullptr), address(0) {

}

Mem::Mem(const Mem &obj) {
    name =
        obj.name != nullptr
        ? new string(*(obj.name))
        : nullptr;
        
    type = obj.type;
    
    array =
        obj.array != nullptr
        ? new Array(*(obj.array))
        : nullptr;

    address = obj.address;
}

Mem::~Mem() { 
    delete name;
    name = nullptr;
    
    delete array;
    array = nullptr;
}

string Mem::str() {
    stringstream ss;
    
    ss << "[";
    ss << "name: " << (name != nullptr ? *name : "NULL") << "; ";
    ss << "type: " << type << "; ";
    ss << "array: " << (array != nullptr ? array->str() : "NULL") << "; ";
    ss << "address: " << address << ";";
    ss << "]";
    
    return ss.str();
}


/* ---------------------------------------------------------------------------------------------*/

vector<Mem*> memory; 
int memory_current = 0;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug() {
    cout << "[MEMORY]\n";
    for(auto mem : memory) {
        std::cout << mem->str() << "\n";
    }
}

/* ---------------------------------------------------------------------------------------------*/

void mem_free() {
    for(auto mem : memory) {
        delete mem;
    }
}

