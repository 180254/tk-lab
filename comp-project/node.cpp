#include "global.hpp"

/* ---------------------------------------------------------------------------------------------*/

std::ostream& operator<< (std::ostream& os, TypeEnum te) {
    switch (te) {
        case TypeEnum::TE_UNKNOWN: return os << "TE_UNKNOWN" ;
        case TypeEnum::TE_INTEGER: return os << "TE_INTEGER";
        case TypeEnum::TE_REAL:    return os << "TE_REAL" ;
        case TypeEnum::TE_ARRAY:   return os << "TE_ARRAY" ;
        case TypeEnum::TE_BOOLEAN: return os << "TE_BOOLEAN" ;
        case TypeEnum::TE_SPEC:    return os << "TE_SPEC" ;
        case TypeEnum::TE_ERROR:   return os << "TE_ERROR" ;
    };
    return os << static_cast<std::uint16_t>(te);
}

/*----------------------------------------------------------------------------------------------*/

Type::Type() : te(TE_UNKNOWN), array(nullptr) {
}

Type::Type(const Type &other) {
    te = other.te;
    
    if(other.array != nullptr) {
        array = new Array(*other.array);
    } else {
        array = nullptr;
    }
}

Type::~Type() {
    DELETE(array);
}


bool Type::operator==(const Type& other) {
    return te == other.te
        && (
            te == TE_ARRAY
                ? (*array == *(other.array))
                : true
        );
}

string Type::str() {
    stringstream ss;
    ss << te;
    ss << (array == nullptr ? "" : ("(" + array->str() + ")"));
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
    ss << te << "|";
    ss << min << "|";
    ss << max;
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

Symbol::Symbol() : name(nullptr), type(nullptr), offset(-1), reference(false) {
}

Symbol::Symbol(const Symbol &other) {
    if(other.name != nullptr) {
        name = new string(*(other.name));
    } else {
        name = nullptr;
    }
    
    if(other.type != nullptr) {
        type = new Type(*(other.type));
    } else {
        type = nullptr;
    }
    
    offset = other.offset;
    reference = other.reference;
}

Symbol::~Symbol()  {
   DELETE(name);
   DELETE(type);
}

/*----------------------------------------------------------------------------------------------*/

string Symbol::str() {
    stringstream ss;
    ss << (name == nullptr ? "?" : *name) << "|";
    ss << (type == nullptr ? "?" : type->str()) << "|";
    ss << offset << "|";
    ss << reference;
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

std::ostream& operator<< (std::ostream& os, Operation oper)
{
    switch (oper)
    {
        case Operation::OP_UNKNOWN :        return os << "OP_UNKNOWN" ;
        case Operation::OP_ID:              return os << "OP_ID" ;
        case Operation::OP_CONSTANT:        return os << "OP_CONSTANT" ;
        case Operation::OP_ASSIGN:          return os << "OP_ASSIGN" ;
        case Operation::OP_ARRAY_ACCESS:    return os << "OP_ARRAY_ACCESS" ;
        case Operation::OP_MATH_PLUS:       return os << "OP_MATH_PLUS" ;
        case Operation::OP_MATH_MINUS:      return os << "OP_MATH_MINUS" ;
        case Operation::OP_MATH_UMINUS:     return os << "OP_MATH_UMINUS" ;
        case Operation::OP_MATH_UPLUS:      return os << "OP_MATH_UPLUS" ;
        case Operation::OP_MATH_MOD:        return os << "OP_MATH_MOD" ;
        case Operation::OP_MATH_DIV1:       return os << "OP_MATH_DIV1" ;
        case Operation::OP_MATH_DIV2:       return os << "OP_MATH_DIV2" ;
        case Operation::OP_MATH_MUL:        return os << "OP_MATH_MUL" ;
        case Operation::OP_FLOW_IF:         return os << "OP_FLOW_IF" ;
        case Operation::OP_FLOW_IF_THEN:    return os << "OP_FLOW_IF_THEN" ;
        case Operation::OP_FLOW_WHILE:      return os << "OP_FLOW_WHILE" ;
        case Operation::OP_CALL_FUNC:       return os << "OP_CALL_FUNC" ;
        case Operation::OP_LOG_NOT:         return os << "OP_LOG_NOT" ;
        case Operation::OP_LOG_AND:         return os << "OP_LOG_AND" ;
        case Operation::OP_LOG_OR:          return os << "OP_LOG_OR" ;
        case Operation::OP_LOG_NE:          return os << "OP_LOG_NE" ;
        case Operation::OP_LOG_LE:          return os << "OP_LOG_LE" ;
        case Operation::OP_LOG_GE:          return os << "OP_LOG_GE" ;
        case Operation::OP_LOG_LO:          return os << "OP_LOG_LO" ;
        case Operation::OP_LOG_GR:          return os << "OP_LOG_GR" ;
        case Operation::OP_LOG_EQ:          return os << "OP_LOG_EQ" ;
    };
    return os << static_cast<std::uint16_t>(oper);
}

/* ---------------------------------------------------------------------------------------------*/

Expression::Expression() :
    oper(OP_UNKNOWN),
    args(new vector<ExprArg*>()),
    result(-1),
    line (-1) {
}

Expression::Expression(Operation operation) :
    oper(operation),
    args(new vector<ExprArg*>()),
    result(-1),
    line(yylineno) {
}

Expression::~Expression() {
    for(auto arg : *args) {
        DELETE(arg);
    }
    
    DELETE(args);
}

string Expression::str(int level) {
    
    stringstream ss;
    ss << string(level, ' ');
    ss << line << "|";
    ss << oper << "|";
    ss << result;
       
    for(auto exp_arg : *args) {
        ss << "\n" << exp_arg->str(level+1);
    }
    
    return ss.str();
}

/* ---------------------------------------------------------------------------------------------*/

std::ostream& operator<< (std::ostream& os, ExprArgType eat) {
    switch (eat) {
        case ExprArgType::E_UNKNOWN:        return os << "E_UNKNOWN" ;
        case ExprArgType::E_ID_S:           return os << "E_ID_S";
        case ExprArgType::E_CONSTANT_S:     return os << "E_CONSTANT_S" ;
        case ExprArgType::E_EXPRESSION:     return os << "E_EXPRESSION" ;
        case ExprArgType::E_EXPRESSION_V:   return os << "E_EXPRESSION_V" ;

    };
    return os << static_cast<std::uint16_t>(eat);
}

/* ---------------------------------------------------------------------------------------------*/

ExprArg::ExprArg() : type(E_UNKNOWN) {
}

ExprArg::~ExprArg() {

    if(type == E_UNKNOWN) {
        
    } else if(type == E_ID_S) {
        DELETE(val.sVal);
    
    } else if(type == E_CONSTANT_S) {
        DELETE(val.sVal);
        
    } else if(type == E_EXPRESSION) {
        DELETE(val.eVal);
    
    } else if(type == E_EXPRESSION_V) {
        for(auto exp : *(val.evVal)) {
            DELETE(exp);
        }
        DELETE(val.evVal);
    }
}

string ExprArg::str(int level) {

    stringstream ss;
    ss << string(level, ' ');
    ss << type;
    
    if(type == E_UNKNOWN) {
        ss << "|" << "0";
        
    } else if(type == E_ID_S) {
        ss << "|" << *(val.sVal);
    
    } else if(type == E_CONSTANT_S) {
        ss << "|" << *(val.sVal);
        
    } else if(type == E_EXPRESSION) {
        ss << "\n" << val.eVal->str(level+1);
    
    } else if(type == E_EXPRESSION_V) {
        for(auto exp : *(val.evVal)) {
            ss << "\n" << exp->str(level+1);
        }
    }
    
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

Function::Function() :
    name(nullptr),
    args(nullptr),
    result(nullptr),
    stack(nullptr),
    body(nullptr) {
}

Function::~Function() {
    DELETE(name);
    
    for(auto arg : *args) {
        DELETE(arg);
    }
    DELETE(args);
    
    DELETE(result);
    
    for(auto sym : *stack) {
        DELETE(sym);
    }
    DELETE(stack);
    
    for(auto expr : *body) {
        DELETE(expr);
    }
    DELETE(body);
}

string Function::str(int level) {

    stringstream ss;
    ss << string(level, ' ');
    
    ss << *name << "\n";
   
    for(auto arg : *args) {
        ss << " p|" << arg->str() << "\n";
    }
   
    ss << " r|" << (result != nullptr ? result->str() : "_none_");
    
    for(auto symbol : *stack) {
        ss << "\n" << " m|" << symbol->str();
    }

    for(auto expr : *body) {
        ss << "\n" << expr->str(0) ;
    }
    
    
    return ss.str();
}
    
/* ---------------------------------------------------------------------------------------------*/

vector<Symbol*>             memory;
vector<Function*>           functions;
vector<Expression*>         program;

/* ---------------------------------------------------------------------------------------------*/

int mem_find(vector<Symbol*> v_sym, string str) {

    for(size_t i = 0; i < v_sym.size(); i++) {
        if(*(v_sym[i]->name) == str) {
            return i;
        }
    }
    
    return -1;
}

/* ---------------------------------------------------------------------------------------------*/

int mem_add(vector<Symbol*>, Symbol*) {
    return 1;
}

/* ---------------------------------------------------------------------------------------------*/

void mem_debug() {
    cout << "[MEMORY]" << "\n";
    for(auto symbol : memory) {
        cout << symbol->str() << "\n";
    }
    cout << "\n";
    
    cout << "[FUNCTIONS]" << "\n";
    for(auto func : functions) {
        cout << func->str(0) << "\n";
    }
    cout << "\n";
    
    cout << "[PROGRAM]" << "\n";
    for(auto expr : program) {
        cout << expr->str(0) << "\n";
    }
    cout << "\n";
}

/* ---------------------------------------------------------------------------------------------*/

void mem_free() {
    for(auto symbol : memory) {
        DELETE(symbol);
    }
    memory.clear();
    
    for(auto func : functions) {
        DELETE(func);
    }
    functions.clear();
    
    for(auto expr : program) {
         DELETE(expr);
    }
    program.clear();
}

