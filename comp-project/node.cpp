#include "global.hpp"

/* ---------------------------------------------------------------------------------------------*/

Symbol::Symbol() :
    name(nullptr),
    type(UNKNOWN),
    info(nullptr),
    offset(-1),
    reference(false) {

}

Symbol::Symbol(const Symbol &obj) {
    if(obj.name != nullptr) {
        name = new string(*(obj.name));
    } else {
        name = nullptr;
    }
        
    type = obj.type;
    
    
    if(type == ARRAY && obj.info != nullptr) {
        info = new Array(*((Array*)obj.info));
    } else {
        info = nullptr;
    }

    offset = obj.offset;
    reference = obj.reference;
}

Symbol::~Symbol() { 
    DELETE(name);
    
    if(type == ARRAY) {
        DELETE_C((Array*)info, info);
    }
}

string Symbol::str() {
    stringstream ss;
    
    ss << "[";
    
    if(name != nullptr) {
        ss << "name: " <<  *name << "; ";
    } else {
        ss << "name: " << "NULL" << "; ";
    }
    
    ss << "type: "    << type << "; ";
    
    if(type == ARRAY && info != nullptr) {
        ss << "info: " << ((Array*)info)->str() << "; ";
    } else {
        ss << "info: " << "NULL" << "; ";
    }
    
    ss << "offset: " << offset << "; ";
    ss << "reference: " << reference << ";";
    ss << "]";
    
    return ss.str();
}


/* ---------------------------------------------------------------------------------------------*/

Array::Array() :
    type(UNKNOWN),
    min(-1),
    max(-1) {

}

Array::Array(const Array &obj) {
    min  = obj.min;
    max  = obj.max;
    type = obj.type;
}

Array::~Array() {

}

string Array::str() {
    stringstream ss;
    
    ss << "[";
    ss << "min: "  << min  << "; ";
    ss << "max: "  << max  << "; ";
    ss << "type: " << type << ";";
    ss << "]";
    
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/



/* ---------------------------------------------------------------------------------------------*/

std::vector<Symbol*> memory;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug() {
    cout << "[MEMORY]" << "\n";
    for(auto symbol : memory) {
        std::cout << symbol->str() << "\n";
    }
}

/* ---------------------------------------------------------------------------------------------*/

void mem_free() {
    for(auto symbol : memory) {
        delete symbol;
    }
}

