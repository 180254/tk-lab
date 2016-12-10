#include "global.hpp"

/* ------------------------------------------------------------------------- */

std::ostream& operator<< (std::ostream& os, TypeEnum te) {
    switch (te) {
        case TE_UNKNOWN: return os << "TE_UNKNOWN";
        case TE_VOID:    return os << "TE_VOID";
        case TE_INTEGER: return os << "TE_INTEGER";
        case TE_REAL:    return os << "TE_REAL";
        case TE_ARRAY:   return os << "TE_ARRAY";
        case TE_BOOLEAN: return os << "TE_BOOLEAN";
        case TE_SPEC:    return os << "TE_SPEC";
        case TE_ERROR:   return os << "TE_ERROR";
    };
    return os << static_cast<int>(te);
}

/* ------------------------------------------------------------------------- */

Type::Type() : te(TE_UNKNOWN), array(nullptr), reference(false) {
}

Type::Type(const Type& other) {
    te = other.te;

    if(other.array != nullptr) {
        array = new Array(*other.array);
    } else {
        array = nullptr;
    }

    reference = other.reference;
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
        // && reference == other.reference;
}

string Type::str() {
    stringstream ss;
    ss << te;
    ss << (array == nullptr ? "" : ("(" + array->str() + ")")) << "|";
    ss << (reference ? "R" : "N");
    return ss.str();
}

/* ------------------------------------------------------------------------- */

int type_size(Type* type) {
    if (type->reference) return 4;

    switch (type->te) {
        case TE_UNKNOWN: return 0;
        case TE_VOID:    return 0;
        case TE_INTEGER: return 4;
        case TE_REAL:    return 8;
        case TE_ARRAY:   {
            Array* array = type->array;
            Type arType; arType.te = array->te;
            return (array->max - array->min + 1) * type_size(&arType);
        }
        case TE_BOOLEAN: return 4; // ? stored as int
        case TE_SPEC:    return 4;
        case TE_ERROR:   return 0;
    };
}

/* ------------------------------------------------------------------------- */

bool type_is_num(Type* type) {
    return type_eff(type) == TE_INTEGER || type_eff(type) == TE_REAL;
}

/* ------------------------------------------------------------------------- */

TypeEnum type_eff(Type* type) {
    return type->te != TE_ARRAY
        ? type->te
        : type->array->te;
}


/* ------------------------------------------------------------------------- */

Array::Array() : te(TE_UNKNOWN), min(-1), max(-1) {
}

Array::Array(const Array& other) {
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

/* ------------------------------------------------------------------------- */

Symbol::Symbol() :
    name(nullptr),
    type(nullptr),
    offset(-1),
    level(0) {
}

Symbol::Symbol(const Symbol& other) {
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
    level = other.level;
}

Symbol::~Symbol()  {
   DELETE(name);
   DELETE(type);
}

/* ------------------------------------------------------------------------- */

string Symbol::str() {
    stringstream ss;
    ss << (name == nullptr ? "?" : *name) << "|";
    ss << (type == nullptr ? "?" : type->str()) << "|";
    ss << offset << "|";
    ss << level;
    return ss.str();
}

/* ------------------------------------------------------------------------- */

std::ostream& operator<< (std::ostream& os, Operation oper) {
    switch (oper) {
        case OP_UNKNOWN :        return os << "OP_UNKNOWN";
        case OP_ID:              return os << "OP_ID";
        case OP_CONSTANT:        return os << "OP_CONSTANT";
        case OP_ASSIGN:          return os << "OP_ASSIGN";
        case OP_ARRAY_ACCESS:    return os << "OP_ARRAY_ACCESS";
        case OP_MATH_PLUS:       return os << "OP_MATH_PLUS";
        case OP_MATH_MINUS:      return os << "OP_MATH_MINUS";
        case OP_MATH_UMINUS:     return os << "OP_MATH_UMINUS";
        case OP_MATH_UPLUS:      return os << "OP_MATH_UPLUS";
        case OP_MATH_MOD:        return os << "OP_MATH_MOD";
        case OP_MATH_DIV1:       return os << "OP_MATH_DIV1";
        case OP_MATH_DIV2:       return os << "OP_MATH_DIV2";
        case OP_MATH_MUL:        return os << "OP_MATH_MUL";
        case OP_FLOW_IF:         return os << "OP_FLOW_IF";
        case OP_FLOW_IF_THEN:    return os << "OP_FLOW_IF_THEN";
        case OP_FLOW_WHILE:      return os << "OP_FLOW_WHILE";
        case OP_CALL_FUNC:       return os << "OP_CALL_FUNC";
        case OP_LOG_NOT:         return os << "OP_LOG_NOT";
        case OP_LOG_AND:         return os << "OP_LOG_AND";
        case OP_LOG_OR:          return os << "OP_LOG_OR";
        case OP_LOG_NE:          return os << "OP_LOG_NE";
        case OP_LOG_LE:          return os << "OP_LOG_LE";
        case OP_LOG_GE:          return os << "OP_LOG_GE";
        case OP_LOG_LO:          return os << "OP_LOG_LO";
        case OP_LOG_GR:          return os << "OP_LOG_GR";
        case OP_LOG_EQ:          return os << "OP_LOG_EQ";
    };
    return os << static_cast<int>(oper);
}

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

std::ostream& operator<< (std::ostream& os, ExprArgType eat) {
    switch (eat) {
        case E_UNKNOWN:        return os << "E_UNKNOWN";
        case E_ID_S:           return os << "E_ID_S";
        case E_CONSTANT_S:     return os << "E_CONSTANT_S";
        case E_EXPRESSION:     return os << "E_EXPRESSION";
        case E_PROGRAM:        return os << "E_PROGRAM";
    };
    return os << static_cast<int>(eat);
}

/* ------------------------------------------------------------------------- */

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

    } else if(type == E_PROGRAM) {
        for(auto exp : *(val.pVal)) {
            DELETE(exp);
        }
        DELETE(val.pVal);
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

    } else if(type == E_PROGRAM) {
        for(auto exp : *(val.pVal)) {
            ss << "\n" << exp->str(level+1);
        }
    }

    return ss.str();
}

/* ------------------------------------------------------------------------- */

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

ExprArg* expr_arg_prog(Program* arg) {
    auto var = new ExprArg();
    var->type = E_PROGRAM;
    var->val.pVal = arg;
    return var;
}

/* ------------------------------------------------------------------------- */

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

    for(auto symbol : *stack->vec) {
        ss << "\n" << " m|" << symbol->str();
    }

    for(auto expr : *body) {
        ss << "\n" << expr->str(0) ;
    }

    return ss.str();
}
/* ------------------------------------------------------------------------- */

Memory::Memory() : vec(new vector<Symbol*>()),
                   offset_asc(true) {
}

Memory::~Memory() {
    for(auto symbol : *vec) {
        DELETE(symbol);
    }
    DELETE(vec);
}

/* ------------------------------------------------------------------------- */

Memory               memory;
vector<Function*>    functions;
Program              program;

/* ------------------------------------------------------------------------- */

int mem_find(Memory* mem, string str) {
    for(size_t i = 0; i < mem->vec->size(); i++) {
        if(*(mem->vec->at(i)->name) == str) {
            return i;
        }
    }

    return -1;
}

/* ------------------------------------------------------------------------- */

int mem_add(Memory* mem, Symbol* sym, int offset) {
    if(offset == 0 && mem->vec->size() > 0) {
        auto last_sym = mem->vec->at(mem->vec->size()-1);
        auto asc_mul = mem->offset_asc ? 1 : -1;
        sym->offset = last_sym->offset + asc_mul * type_size(last_sym->type);

    } else {
        sym->offset = offset;
    }

    mem->vec->push_back(sym);
    return mem->vec->size()-1;
}

/* ------------------------------------------------------------------------- */

int mem_temp(Memory* mem, Type* type) {
    Symbol* sym = new Symbol();
    sym->name = new string("$t" + to_string(mem->vec->size()));
    sym->type = new Type();
    sym->type->te = type_eff(type);

    if(mem != &memory) {
        sym->level = 1;
    }

    return mem_add(mem, sym, 0);
}

/* ------------------------------------------------------------------------- */

int func_find(string str) {
    for(size_t i = 0; i < functions.size(); i++) {
        if(*(functions.at(i)->name) == str) {
            return i;
        }
    }

    return -1;
}

/* ------------------------------------------------------------------------- */

void mem_debug() {
    cout << "[MEMORY]" << "\n";
    for(auto symbol : *(memory.vec)) {
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

/* ------------------------------------------------------------------------- */

void mem_free() {
    for(auto symbol : *(memory.vec)) {
        DELETE(symbol);
    }
    memory.vec->clear();

    for(auto func : functions) {
        DELETE(func);
    }
    functions.clear();

    for(auto expr : program) {
         DELETE(expr);
    }
    program.clear();
}

/* ------------------------------------------------------------------------- */
