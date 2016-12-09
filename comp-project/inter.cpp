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
            
            attr->place = sym_to_place(mem, sym_id);
            attr->type = new Type(*(mem->vec->at(sym_id)->type));
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_CONSTANT:
        {
            attr->place = new string("#" + *(expr->args->at(0)->val.sVal));
            bool integer = attr->place->find(".") == std::string::npos;
            attr->type = new Type();
            attr->type->te = integer ? TE_INTEGER : TE_REAL;
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_ASSIGN:
        {
            Expression* arg1 = expr->args->at(0)->val.eVal;
            Expression* arg2 = expr->args->at(1)->val.eVal;
            
            Attr* arg1a = compute(arg1, mem);
            Attr* arg2a = compute(arg2, mem);
            
                        for(auto code : *(arg1a->code)) {
            attr->code->push_back(code);
        }
                    for(auto code : *(arg2a->code)) {
            attr->code->push_back(code);
        }
        
            if(!type_is_num(arg1a->type) || !type_is_num(arg2a->type)) {
                attr_set_error(attr);
                cerr << "sem err, line=" << expr->line << ": "
                     << "bad assign operand" << "\n";
                break;
            }
            
            if(arg1a->type->te == TE_INTEGER && arg2a->type->te == TE_REAL) {
                string* cast_c = cast(arg2a, TE_INTEGER, mem);
                attr->code->push_back(cast_c);
            }
            
            if(arg1a->type->te == TE_REAL && arg2a->type->te == TE_INTEGER) {
                string* cast_c = cast(arg2a, TE_REAL, mem);
                attr->code->push_back(cast_c);
            }
            
            attr->type  = new Type();
            attr->type->te = TE_VOID;
            
            string* assign_asm = asm_gen("mov", arg2a, arg1a);
            attr->code->push_back(assign_asm);
        }
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_ARRAY_ACCESS:
        
        break;
        
        /* ----------------------------------------------------------------- */
        
        case OP_MATH_PLUS:
        {
            Expression* arg1 = expr->args->at(0)->val.eVal;
            Expression* arg2 = expr->args->at(1)->val.eVal;
            
            Attr* arg1a = compute(arg1, mem);
            Attr* arg2a = compute(arg2, mem);
            
                for(auto code : *(arg1a->code)) {
            attr->code->push_back(code);
        }
                    for(auto code : *(arg2a->code)) {
            attr->code->push_back(code);
        }
            if(!type_is_num(arg1a->type) || !type_is_num(arg2a->type)) {
                attr_set_error(attr);
                cerr << "sem err, line=" << expr->line << ": "
                     << "bad + operand" << "\n";
                break;
            }
            
            if(arg1a->type->te == TE_INTEGER && arg2a->type->te == TE_REAL) {
                string* cast_c = cast(arg1a, TE_REAL, mem);
                attr->code->push_back(cast_c);
            }
            
            if(arg1a->type->te == TE_REAL && arg2a->type->te == TE_INTEGER) {
                string* cast_c = cast(arg2a, TE_REAL, mem);
                attr->code->push_back(cast_c);
            }
            
            int temp_id = mem_temp(mem, arg1a->type->te);
            attr->place = sym_to_place(mem, temp_id);
                   
            attr->type  = new Type();
            attr->type->te = arg1a->type->te;
            
            string* op_asm = asm_gen("add", arg1a, arg2a, attr);
            attr->code->push_back(op_asm);
        }
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
        {
            string* f_name = expr->args->at(0)->val.sVal;
            
            if(*f_name == "write") {
                for(int i = 1; i < expr->args->size(); i++) {
                    Expression* arg = expr->args->at(i)->val.eVal;
                    Attr* arg_a = compute(arg, mem);
                    
                    attr->type  = new Type();
                    attr->type->te = TE_VOID;
                    
                    string* assign_asm = asm_gen("write", arg_a);
                    attr->code->push_back(assign_asm);
                }
         
            }
        }
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

Attr* compute(vector<Expression*>* v_expr, Memory* mem) {
    Attr* attr = new Attr();
    attr->type = new Type();
    attr->type->te = TE_VOID;
    
    for(auto expr: *v_expr) {
        Attr* c_attr = compute(expr, mem);
        for(auto code : *(c_attr->code)) {
            attr->code->push_back(code);
        }
    }
    
    return attr;
}

/* --------------------------------------------------------------------------*/

void attr_set_error(Attr* attr) {
    attr->place = new string("");
    attr->type->te = TE_ERROR;
}

/* --------------------------------------------------------------------------*/

string* attr_to_code(Attr* attr) {
    stringstream ss;
    
    for(auto code: *(attr->code)) {
        ss << *code << "\n";
    }
    
    return new string(ss.str());
}

/* --------------------------------------------------------------------------*/

string* sym_to_place(Memory* mem, int sym_index) {
    return sym_to_place(mem->vec->at(sym_index));
}

/* --------------------------------------------------------------------------*/

string* sym_to_place(Memory* mem, string sym_name) {
     int index = mem_find(mem, sym_name);
     return sym_to_place(mem, index);
}

/* --------------------------------------------------------------------------*/

string* sym_to_place(Symbol* sym) {
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
    return new string(ss.str());
}

/* --------------------------------------------------------------------------*/

string* cast(Attr* attr, TypeEnum te, Memory* mem) {
    string func = te == TE_INTEGER ? "realtoint.r" : "inttoreal.i";
    
    int temp_id = mem_temp(mem, TE_INTEGER);
    string* temp_place = sym_to_place(mem, temp_id);
    
   
    
    
    string code = func + " " + *(attr->place) + "," + *temp_place;
    
     DELETE(attr->place);
    attr->place = temp_place;
    attr->type->te = te;
    
    
    
    return new string(code);
}

/* --------------------------------------------------------------------------*/

string* asm_gen(string command, Attr* arg1, Attr* arg2, Attr* arg3) {
    
    string ir = arg1->type->te == TE_INTEGER ? "i" : "r";
    string code = command + "." + ir + " " + *arg1->place;
    
    if(arg2 != nullptr) {
        code += "," + *arg2->place;
    }
    
    if(arg3 != nullptr) {
        code += "," + *arg3->place;
    }
    
    return new string(code);
}

/* ------------------------------------------------------------------------- */

void asm_app_gen() {
    cout << *attr_to_code(compute(&program, &memory)) << "\n";
}


/* --------------------------------------------------------------------------*/

int lab_current = 0;
string* lab_next() {
    stringstream ss;
    ss << "lab" << to_string(lab_current);
    return new string(ss.str());
}

/* --------------------------------------------------------------------------*/
