#include "global.hpp"

Attr::Attr() : code(new vector<string*>()),
               place(nullptr),
               type(nullptr) {
}

/* --------------------------------------------------------------------------*/

Attr* compute(Expression* expr, vector<Symbol*>* e_mem) {
    
    Attr* attr = new Attr();

    switch(expr->oper) {
        case OP_UNKNOWN:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_ID:
        {
            string* sym_name = expr->args->at(0)->val.sVal;
            int sym_id = mem_find(*e_mem, *sym_name);
            
            if(sym_id == -1) {
                set_attr_error(attr);
                cerr << "semerr: unknown variable " << *sym_name
                     << " (line: " << expr->line << ")" << "\n";
                break;
            }
            
            attr->place = new string(sym_to_place(e_mem->at(sym_id)));
            attr->type = new Type(*(e_mem->at(sym_id)->type));
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

void set_attr_error(Attr* attr) {
    attr->place = new string("");
    attr->type->te = TE_ERROR;
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
