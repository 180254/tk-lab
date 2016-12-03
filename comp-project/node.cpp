#include "global.h"

Array::Array() : min(-1), max(-1), var_type(UNKNOWN) {

}

Array::Array(const Array &obj) {
    min = obj.min;
    max = obj.max;
    var_type = obj.var_type;
}

Array::~Array() {

}

std::string Array::to_string() {
    std::stringstream ss;
   ss << "[";
    ss << "min: " << min << "; ";
    ss << "max: " << max << "; ";
    //ss << "var_type: " << var_type << "; ";
    ss << "]";
    return ss.str();
}



/* ---------------------------------------------------------------------------------------------*/

Mem::Mem() : name(nullptr), var_type(UNKNOWN), array(nullptr), address(0) {

}

Mem::Mem(const Mem &obj) {
    if(name != nullptr)
        name = new std::string(*(obj.name));
    var_type = obj.var_type;
    if(array != nullptr)
        array = new Array(*(obj.array));
    address = obj.address;
}

Mem::~Mem() { 
    delete name;
    name = nullptr;
    delete array;
    array = nullptr;
}

std::string Mem::to_string() {
    std::stringstream ss;
    
    ss << "[";
      if(name != nullptr)
    ss << "name: " << *name << "; ";
    ss << "var_type: " << var_type << "; ";
    if(array != nullptr)
    ss << "array: " << array->to_string() << "; ";
    ss << "address: " << address << "; ";
    ss << "]";
    
    return ss.str();
}


/* ---------------------------------------------------------------------------------------------*/

int mem_ptr = 0;
std::vector<Mem*> memory; 


