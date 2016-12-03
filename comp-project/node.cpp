#include "global.h"

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

std::string Array::to_string() {
    std::stringstream ss;
    
    ss << "[";
    ss << "min: " << min << "; ";
    ss << "max: " << max << "; ";
    ss << "type: " << type << ";";
    ss << "]";
    
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

Mem::Mem() : name_ptr(nullptr), type(UNKNOWN), array_ptr(nullptr), address(0) {

}

Mem::Mem(const Mem &obj) {
    if(name_ptr != nullptr)
        name_ptr = new std::string(*(obj.name_ptr));

    type = obj.type;
    
    if(array_ptr != nullptr)
        array_ptr = new Array(*(obj.array_ptr));

    address = obj.address;
}

Mem::~Mem() { 
    delete name_ptr;
    name_ptr = nullptr;
    
    delete array_ptr;
    array_ptr = nullptr;
}

std::string Mem::to_string() {
    std::stringstream ss;
    
    ss << "[";
    ss << "name_ptr: " << (name_ptr != nullptr ? *name_ptr : "NULL") << "; ";
    ss << "var_type: " << type << "; ";
    ss << "array_ptr: " << (array_ptr != nullptr ? array_ptr->to_string() : "NULL") << "; ";
    ss << "address: " << address << "; ";
    ss << "]";
    
    return ss.str();
}


/* ---------------------------------------------------------------------------------------------*/

int mem_ptr = 0;
std::vector<Mem*> memory; 


