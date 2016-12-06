#include "global.hpp"

/* ---------------------------------------------------------------------------------------------*/

std::ostream& operator<< (std::ostream & os, TypeEnum eth)
{
    switch (eth)
    {
        case  TypeEnum::TE_UNKNOWN: return os << "TE_UNKNOWN" ;
        case  TypeEnum::TE_INTEGER: return os << "TE_INTEGER";
        case  TypeEnum::TE_REAL:    return os << "TE_REAL" ;
        case  TypeEnum::TE_ARRAY:   return os << "TE_ARRAY" ;
        case  TypeEnum::TE_BOOLEAN: return os << "TE_BOOLEAN" ;
        case  TypeEnum::TE_ERROR:   return os << "TE_ERROR" ;
    };
    return os << static_cast<std::uint16_t>(eth);
}

/*----------------------------------------------------------------------------------------------*/

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

std::ostream& operator<< (std::ostream & os, Operation eth)
{
    switch (eth)
    {

        case      Operation::OP_UNKNOWN : return os << "OP_UNKNOWN" ;
        case   Operation::OP_ID: return os << "OP_ID" ;
        case   Operation::OP_CONSTANT: return os << "OP_CONSTANT" ;
        case   Operation::OP_ASSIGN: return os << "OP_ASSIGN" ;
        case   Operation::OP_ARRAY_ACCESS: return os << "OP_ARRAY_ACCESS" ;
        case   Operation::OP_MATH_PLUS: return os << "OP_MATH_PLUS" ;
        case   Operation::OP_MATH_MINUS: return os << "OP_MATH_MINUS" ;
        case   Operation::OP_MATH_UMINUS: return os << "OP_MATH_UMINUS" ;
        case   Operation::OP_MATH_UPLUS: return os << "OP_MATH_UPLUS" ;
       case    Operation::OP_MATH_MOD: return os << "OP_MATH_MOD" ;
        case   Operation::OP_MATH_DIV1: return os << "OP_MATH_DIV1" ;
        case   Operation::OP_MATH_DIV2: return os << "OP_MATH_DIV2" ;
        case   Operation::OP_MATH_MUL: return os << "TE_UNKNOWN" ;
        case   Operation::OP_FLOW_IF: return os << "OP_FLOW_IF" ;
        case   Operation::OP_FLOW_IF_THEN: return os << "OP_FLOW_IF_THEN" ;
        case   Operation::OP_FLOW_WHILE: return os << "OP_FLOW_WHILE" ;
        case   Operation::OP_CALL_FUNC: return os << "OP_CALL_FUNC" ;
        case   Operation::OP_LOG_NOT: return os << "OP_LOG_NOT" ;
        case   Operation::OP_LOG_AND: return os << "OP_LOG_AND" ;
        case   Operation::OP_LOG_OR: return os << "OP_LOG_OR" ;
        case   Operation::OP_LOG_NE: return os << "OP_LOG_NE" ;
        case   Operation::OP_LOG_LE: return os << "OP_LOG_LE" ;
        case   Operation::OP_LOG_GE: return os << "OP_LOG_GE" ;
        case   Operation::OP_LOG_LO: return os << "OP_LOG_LO" ;
        case   Operation::OP_LOG_GR: return os << "OP_LOG_GR" ;
       case    Operation::OP_LOG_EQ: return os << "OP_LOG_EQ" ;
    };
    return os << static_cast<std::uint16_t>(eth);
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
    return str(0);
}

string Expression::str(int level) {
    string tab = string(level, ' ');
    
    stringstream ss;
    ss << tab
       << "[line: " << line
       << "; oper = " << oper
       << "; result = " << result
       << "; args=\n";
       
    for(auto exp_arg : *args) {
        ss << exp_arg->str(level+1);
    }
    
    ss.seekp(-1, std::ios_base::end);
    ss << "]" << "\n";
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

ExprArg::ExprArg() : type(E_UNKNOWN) {
}



ExprArg::~ExprArg() {
    if(type == E_EXPRESSION) {
        DELETE(val.eVal);
    }
}

string ExprArg::str(int level) {
    string tab = string(level, ' ');
    
    stringstream ss;
    ss << tab
       << "[arg!"
       << "; type = " << type;
    
    if(type == E_UNKNOWN) {
        ss << "; val = 0";
        
    } else if(type == E_ID_S) {
        ss << "; val = " << *(val.sVal);
    
    } else if(type == E_CONSTANT_S) {
        ss << "; val = " << *(val.sVal);
        
    } else if(type == E_EXPRESSION) {
        ss << "; val = \n";
        ss << val.eVal->str(level+1);
    
    } else if(type == E_EXPRESSION_V) {
        ss << "; val = \n";
        for(auto exp : *(val.evVal)) {
            ss << exp->str(level+1);
        }

    }
    
    ss << "]" << "\n";
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

ExprArg* expr_arg_id(string* arg) {
    auto var = new ExprArg();
    var->type = E_ID_S;
    var->val.sVal = arg;
    return var;
}

ExprArg* expr_arg_const(string* arg) {
    auto var = new ExprArg();
    var->type = E_CONSTANT_S;
    var->val.sVal = arg;
    return var;
}

ExprArg* expr_arg_expr(Expression* arg) {
    auto var = new ExprArg();
    var->type = E_EXPRESSION;
    var->val.eVal = arg;
    return var;
}

ExprArg* expr_arg_expr_v(vector<Expression*>* arg) {
    auto var = new ExprArg();
    var->type = E_EXPRESSION_V;
    var->val.evVal = arg;
    return var;
}

/* ---------------------------------------------------------------------------------------------*/

vector<Symbol*>             memory;
extern vector<Function*>    functions;

/* ---------------------------------------------------------------------------------------------*/

void mem_debug() {
    cout << "[MEMORY]" << "\n";
    for(auto symbol : memory) {
        cout << symbol->str() << "\n";
    }
}

/* ---------------------------------------------------------------------------------------------*/

void mem_free() {
}

/* ---------------------------------------------------------------------------------------------*/

int mem_find(vector<Symbol*> v_sym, string str) {

    for(size_t i = 0; i < v_sym.size(); i++) {
        if(*(v_sym[i]->name) == str) {
            return i;
        }
    }
    
    return -1;
}

