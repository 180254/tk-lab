#include "global.hpp"

/* ---------------------------------------------------------------------------------------------*/

Type::Type() : te(TE_UNKNOWN), array(nullptr) {
}

Type::Type(const Type &other) {
    te = other.te;
    
    if(other.array != nullptr) {
        array = new Array(*array);
    } else {
        array = nullptr;
    }
}


Type::~Type() {
    DELETE(array);
}

bool Type::operator==(const Type& other) {
    return te == other.te &&
        (array == nullptr ? other.array == nullptr : (*array == *(other.array)));
}

string Type::str() {
    stringstream ss;
    ss << "["
       << "te: "   << te
       << "; array:" << (array == nullptr ? "NULL" : array->str())
       << "]";
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

Array::Array() : te(TE_UNKNOWN), min(-1), max(-1) {
}

Array::Array(const Array &other) {
    te = other.te;
    min = other.min;
    max = other.max;
}

Array::~Array() {
}

bool Array::operator==(const Array& other) {
    return te == other.te && min == other.min && max == other.max;
}
    
string Array::str() {
    stringstream ss;
    ss << "["
       << "te: "  << te 
       << "; min: " << min 
       << "; max: " << max
       << "]";
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

Symbol::Symbol() : name(nullptr), type(nullptr), offset(-1), reference(false) {
}

Symbol::~Symbol()  {
   DELETE(name);
   DELETE(type);
}

string Symbol::str() {
    stringstream ss;
    ss << "[name: "       << (name == nullptr ? "NULL" : *name)
       << "; type: "      << (type == nullptr ? "NULL" : type->str()) 
       << "; offset: "    << offset
       << "; reference: " << reference
       << "]";
    return ss.str();
}


/* ---------------------------------------------------------------------------------------------*/

Expression::Expression() :
    oper(OP_UNKNOWN),
    args(new vector<ExprArg*>()),
    result(-1),
    line (-1) {
}

Expression::~Expression() {
    for(auto arg : *args) {
        delete arg;
    }
    
    DELETE(args);
}

string Expression::str() {
    return "";
}

/* ---------------------------------------------------------------------------------------------*/

ExprArg::ExprArg() : type(E_UNKNOWN), val(nullptr) {
}


ExprArg::~ExprArg() {
    if(type == E_EXPRESSION) {
        DELETE(val->eVal);
    }
    
    DELETE(val);
}

/* ---------------------------------------------------------------------------------------------*/

std::vector<Symbol*>             memory;
extern std::vector<Function*>    functions;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug() {
    cout << "[MEMORY]" << "\n";
    for(auto symbol : memory) {
        std::cout << symbol->str() << "\n";
    }
}

/* ---------------------------------------------------------------------------------------------*/

void mem_free() {
}

