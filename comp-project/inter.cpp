#include "global.hpp"

Attr::Attr() : code(new vector<string*>()),
               place(nullptr),
               type(nullptr) {
}

/* --------------------------------------------------------------------------*/

Attr* compute(Expression* expr, Memory* mem) {
    
    Attr* attr = new Attr();

    switch(expr->oper) {
        case OP_UNKNOWN:
        {
            attr_set_error(attr);
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_ID:
        {
            string* sym_name = expr->args->at(0)->val.sVal;
            int sym_id = mem_find(mem, *sym_name);
            
            if(sym_id == -1) {
                attr_set_error(attr);
                cerr << "sem err, line=" << expr->line << ": "
                     << "unknown variable " << *sym_name << "\n";
                break;
            }
            
            attr->place = new string(sym_to_place(mem->vec->at(sym_id)));
            attr->type = new Type(*(mem->vec->at(sym_id)->type));
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_CONSTANT:
        {
            attr->place = new string("#" + *(expr->args->at(0)->val.sVal));
            bool real = attr->place->find(".") != std::string::npos;
            attr->type = new Type();
            attr->type->te = real ? TE_REAL : TE_INTEGER;
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_ASSIGN:
        {
            Expression* arg1 = expr->args->at(0)->val.eVal;
            Expression* arg2 = expr->args->at(1)->val.eVal;
            
            Attr* attr1 = compute(arg1, mem);
            Attr* attr2 = compute(arg2, mem);
            
            if(!type_is_num(attr1->type) || !type_is_num(attr2->type)) {
                attr_set_error(attr);
                cerr << "sem err, line=" << expr->line << ": "
                     << "bad assign expression" << "\n";
                break;
            }
            
            if(attr1->type->te == TE_INTEGER && attr2->type->te == TE_REAL) {
                int temp_id = mem_temp(mem, TE_INTEGER);
                Symbol* temp = mem->vec->at(temp_id);
            
            }
            
            if(attr1->type->te == TE_REAL && attr2->type->te == TE_INTEGER) {
            
            }
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_ARRAY_ACCESS:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_PLUS:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_MINUS:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_UMINUS:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_UPLUS:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_MOD:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_DIV1:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_DIV2:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_MUL:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_FLOW_IF:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_FLOW_IF_THEN:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_FLOW_WHILE:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_CALL_FUNC:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_NOT:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_AND:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_OR:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_NE:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_LE:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_GE:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_LO:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_GR:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_LOG_EQ:
        
        break;
        
        /* ----------------------------------------------------------------- */
    }
    
    return attr;
}

/* --------------------------------------------------------------------------*/

void attr_set_error(Attr* attr) {
    attr->place = new string("");
    attr->type->te = TE_ERROR;
}

/* --------------------------------------------------------------------------*/

string sym_to_place(Memory* mem, int index) {
    return sym_to_place(mem->vec->at(index));
}

/* --------------------------------------------------------------------------*/

string sym_to_place(Memory* mem, string var_name) {
     int index = mem_find(mem, var_name);
     return sym_to_place(mem, index);
}

/* --------------------------------------------------------------------------*/

string sym_to_place(Symbol* sym) {
    stringstream ss;
    
    if(sym->type->reference) {
        ss << "*";
    }
    
    if(sym->level == 2) {
        ss << "BP";
        
        if(sym->offset > 0) {
            ss << "+";
        }
    }
    
    ss << sym->offset;
    return ss.str();
}

/* --------------------------------------------------------------------------*/

