#include "global.hpp"

Attr::Attr() : code(new vector<string*>()),
               place(nullptr),
               type(nullptr) {
}

Attr::~Attr() {
    for(auto str: *code) {
        DELETE(str);
    }
    DELETE(code);

    DELETE(place);
    DELETE(type);
}

/* --------------------------------------------------------------------------*/

Attr* compute(Expression* expr, Memory* mem, Attr* parent) {

    Attr* attr = new Attr();

    repeat_switch:
    switch(expr->oper) {
        case OP_UNKNOWN:
        {
            attr_set_error(attr);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_ID:
        {
            string* sth_name = expr->args->at(0)->val.sVal;
            int sym_id = mem_find(mem, *sth_name);

            if(sym_id != -1) {
                attr->place = sym_to_place(mem, sym_id);
                attr->type = new Type(*(mem->vec->at(sym_id)->type));
                break;
            }

            int func_id = func_find(*sth_name);
            if(func_id != -1) {
                expr->oper = OP_CALL_FUNC;
                goto repeat_switch;
            }

            if(sym_id == -1 && func_id == -1) {
                attr_set_error(attr);
                string msg = "unknown var/proc/func " + *sth_name;
                sem_error(expr->line, msg.c_str());
            }

        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_CONSTANT:
        {
            attr->place = new string("#" + *(expr->args->at(0)->val.sVal));

            attr->type = new Type();
            auto find_dot = attr->place->find(".");
            bool integer = find_dot == std::string::npos;

            if(!integer) {
                bool only0 = true;
                for(auto i = find_dot+1; i < attr->place->length(); i++) {
                    if(attr->place->at(i) != '0') {
                        only0 = false;
                        break;
                    }
                }

                if(only0) {
                    attr->place->erase(find_dot);
                }
            }

            attr->type->te = integer ? TE_INTEGER : TE_REAL;
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_ASSIGN:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Expression* arg_2 = expr->args->at(1)->val.eVal;

            Attr* attr_1 = compute(arg_1, mem, attr);
            Attr* attr_2 = compute(arg_2, mem, attr);

            if(!type_is_num(attr_1->type) || !type_is_num(attr_2->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "assign operand incorrect");
            }

            if(attr_1->place->at(0) == '#') {
                attr_set_error(attr);
                sem_error(expr->line, "cannot assign to imm value");
            }

            if(type_eff(attr_1->type) != type_eff(attr_2->type)) {
                string* cast_c = cast(attr_2, attr_1->type, mem);
                attr->code->push_back(cast_c);
            }

            attr->type  = new Type();
            attr->type->te = TE_VOID;
            attr->place = new string("");

            string* asm_g = asm_gen("mov", attr_2, attr_1);
            attr->code->push_back(asm_g);

            DELETE(attr_1);
            DELETE(attr_2);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_ARRAY_ACCESS:
        {
            string* sym_name = expr->args->at(0)->val.sVal;
            int sym_id = mem_find(mem, *sym_name);
            Symbol* sym = new Symbol(*(mem->vec->at(sym_id)));

            Expression* arg_1 = expr->args->at(1)->val.eVal;
            Attr* attr_1 = compute(arg_1, mem, attr);

            if(sym_id == -1 || sym->type->te != TE_ARRAY) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect array access");
                break;
            }

            if(type_eff(attr_1->type) != TE_INTEGER) {
                Attr* attr_int = attr_imm(TE_INTEGER, "#0");
                string* cast_c = cast(attr_1, attr_int->type, mem);
                attr->code->push_back(cast_c);
                DELETE(attr_int);
            }

            Attr* attr_asm_1;
            Attr* attr_asm_2;
            Attr* attr_asm_3;
            string* asm_g;

            // sub
            attr_asm_1 = new Attr();
            attr_asm_2 = new Attr();
            attr_asm_3 = new Attr();

            int sub_res = mem_temp(mem, attr_1->type);

            attr_asm_1->type  = new Type(*(attr_1->type));
            attr_asm_1->place = new string(*(attr_1->place));

            int arr_min = sym->type->array->min;
            attr_asm_2->type  = new Type(*(attr_1->type));
            attr_asm_2->place = new string( "#" + to_string(arr_min));

            attr_asm_3->type  = new Type(*(attr_1->type));
            attr_asm_3->place = sym_to_place(mem, sub_res);

            asm_g = asm_gen("sub", attr_asm_1, attr_asm_2, attr_asm_3);
            attr->code->push_back(asm_g);

            DELETE(attr_asm_1);
            DELETE(attr_asm_2);
            DELETE(attr_asm_3);

            // mul
            attr_asm_1 = new Attr();
            attr_asm_2 = new Attr();
            attr_asm_3 = new Attr();

            int mul_res = sub_res;

            attr_asm_1->type  = new Type(*(attr_1->type));
            attr_asm_1->place = sym_to_place(mem, sub_res);

            Type* ar_type = new Type();
            ar_type->te   = sym->type->array->te;

            attr_asm_2->type  = new Type(*(attr_1->type));
            attr_asm_2->place = new string("#" + to_string(type_size(ar_type)));

            attr_asm_3->type  = new Type(*(attr_1->type));
            attr_asm_3->place = sym_to_place(mem, mul_res);

            asm_g = asm_gen("mul", attr_asm_1, attr_asm_2, attr_asm_3);
            attr->code->push_back(asm_g);

            DELETE(ar_type);
            DELETE(attr_asm_1);
            DELETE(attr_asm_2);
            DELETE(attr_asm_3);

            // add
            attr_asm_1 = new Attr();
            attr_asm_2 = new Attr();
            attr_asm_3 = new Attr();

            int add_res = mem_temp(mem, attr_1->type);

            attr_asm_1->type  = new Type(*(attr_1->type));
            attr_asm_1->place = sym_to_place(sym);

            attr_asm_2->type = new Type(*(attr_1->type));
            attr_asm_2->place = sym_to_place(mem, mul_res);

            attr_asm_3->type = new Type(*(attr_1->type));
            attr_asm_3->place = sym_to_place(mem, add_res);

            attr_ref_addr(attr_asm_1);

            asm_g = asm_gen("add", attr_asm_1, attr_asm_2, attr_asm_3);
            attr->code->push_back(asm_g);

            DELETE(attr_asm_1);
            DELETE(attr_asm_2);
            DELETE(attr_asm_3);

            // attr
            mem->vec->at(add_res)->type->reference = true;

            attr->type = new Type();
            attr->type->te = sym->type->array->te;
            attr->place = sym_to_place(mem, add_res);

            DELETE(sym);
            DELETE(attr_1);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_MATH_PLUS:
        case OP_MATH_MINUS:
        case OP_MATH_MOD:
        case OP_MATH_DIV1:
        case OP_MATH_DIV2:
        case OP_MATH_MUL:
        case OP_MATH_AND:
        case OP_MATH_OR:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Expression* arg_2 = expr->args->at(1)->val.eVal;

            Attr* attr_1 = compute(arg_1, mem, attr);
            Attr* attr_2 = compute(arg_2, mem, attr);

            if(!type_is_num(attr_1->type) || !type_is_num(attr_2->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "math operand incorrect");
            }

            if(type_eff(attr_1->type) != type_eff(attr_2->type)) {
                TypeEnum attr1_te = type_eff(attr_1->type);
                Attr* cast_a = attr1_te == TE_INTEGER ? attr_1 : attr_2;
                Attr* cast_b = attr1_te == TE_INTEGER ? attr_2 : attr_1;
                string* cast_c = cast(cast_a, cast_b->type, mem);
                attr->code->push_back(cast_c);
            }

            int tmp_id = mem_temp(mem, attr_1->type);
            attr->place = sym_to_place(mem, tmp_id);
            attr->type = new Type(*(attr_1->type));

            string func;
            switch(expr->oper) {
                case OP_MATH_PLUS:  { func = "add"; break; }
                case OP_MATH_MINUS: { func = "sub"; break; }
                case OP_MATH_MOD:   { func = "mod"; break; }
                case OP_MATH_DIV1:  { func = "div"; break; }
                case OP_MATH_DIV2:  { func = "div"; break; }
                case OP_MATH_MUL:   { func = "mul"; break; }
                case OP_MATH_AND:   { func = "and"; break; }
                case OP_MATH_OR:    { func = "or";  break; }
                default:            { func = "???"; break; }
            }

            string* op_asm = asm_gen(func, attr_1, attr_2, attr);
            attr->code->push_back(op_asm);

            DELETE(attr_1);
            DELETE(attr_2);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_MATH_UMINUS:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Attr* attr_1 = compute(arg_1, mem, attr);

            if(!type_is_num(attr_1->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect uminus");
            };

            int id_res = mem_temp(mem, attr_1->type);

            Attr* attr_asm_1 = new Attr();
            attr_asm_1->type = new Type(*attr_1->type);
            attr_asm_1->place = new string("#0");

            Attr* attr_asm_3 = new Attr();
            attr_asm_3->type = new Type(*attr_1->type);
            attr_asm_3->place = sym_to_place(mem, id_res);

            string* op_asm = asm_gen("sub", attr_asm_1, attr_1, attr_asm_3);
            attr->code->push_back(op_asm);

            attr->place = sym_to_place(mem, id_res);
            attr->type = new Type(*(attr_1->type));

            DELETE(attr_asm_1);
            DELETE(attr_1);
            DELETE(attr_asm_3);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_MATH_UPLUS:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Attr* attr_1 = compute(arg_1, mem, attr);

            if(!type_is_num(attr_1->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect uplus");
            };

            attr->type = new Type(*(attr_1->type));
            attr->place = new string(*(attr_1->place));

            DELETE(attr_1);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_FLOW_IF_THEN:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Attr* attr_1 = compute(arg_1, mem, attr);

            if(!type_is_num(attr_1->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect if condition");
            };

            Program* arg_2 = expr->args->at(1)->val.pVal;
            Program* arg_3 = expr->args->at(2)->val.pVal;

            string* lab1 = lab_next();
            string* lab2 = lab_next();

            Attr* lab1_attr  = attr_imm(TE_INTEGER, "#" + *lab1);
            Attr* lab2_attr  = attr_imm(TE_INTEGER, "#" + *lab2);
            Attr* attr_imm_0 = attr_imm(TE_INTEGER, "#0");

            string* asm_g = asm_gen("je", attr_1, attr_imm_0, lab1_attr);
            attr->code->push_back(asm_g);

            for(auto expr: *arg_2) {
                Attr* attr_x = compute(expr, mem, attr);
                DELETE(attr_x);
            }

            asm_g = asm_gen("jump", lab2_attr);
            attr->code->push_back(asm_g);

            attr->code->push_back(new string(*lab1 + ":"));

            for(auto expr: *arg_3) {
                Attr* attr_x = compute(expr, mem, attr);
                DELETE(attr_x);
            }

            attr->code->push_back(new string(*lab2 + ":"));

            attr->type  = new Type();
            attr->type->te = TE_VOID;
            attr->place = new string("");

            DELETE(attr_1);
            DELETE(lab1);
            DELETE(lab2);
            DELETE(lab1_attr);
            DELETE(lab2_attr);
            DELETE(attr_imm_0);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_FLOW_WHILE:
        {

            string* lab1 = lab_next();
            string* lab2 = lab_next();

            Attr* lab1_attr  = attr_imm(TE_INTEGER, "#" + *lab1);
            Attr* lab2_attr  = attr_imm(TE_INTEGER, "#" + *lab2);
            Attr* attr_imm_0 = attr_imm(TE_INTEGER, "#0");

            attr->code->push_back(new string(*lab2 + ":"));

            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Attr* attr_1 = compute(arg_1, mem, attr);

            if(!type_is_num(attr_1->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect while condition");
            };

            string* asm_g = asm_gen("je", attr_1, attr_imm_0, lab1_attr);
            attr->code->push_back(asm_g);

            Program* arg_2 = expr->args->at(1)->val.pVal;
            for(auto expr: *arg_2) {
                Attr* attr_x = compute(expr, mem, attr);
                DELETE(attr_x);
            }

            asm_g = asm_gen("jump", lab2_attr);
            attr->code->push_back(asm_g);

            attr->code->push_back(new string(*lab1 + ":"));

            DELETE(lab1);
            DELETE(lab2);
            DELETE(lab1_attr);
            DELETE(lab2_attr);
            DELETE(attr_imm_0);
            DELETE(attr_1);
        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_CALL_FUNC:
        {
            string* f_name = expr->args->at(0)->val.sVal;

            if(*f_name == "write" || *f_name == "read") {
                for(size_t i = 1; i < expr->args->size(); i++) {
                    Expression* arg_e = expr->args->at(i)->val.eVal;
                    Attr* attr_e = compute(arg_e, mem, attr);

                    string* asm_g = asm_gen(*f_name, attr_e);
                    attr->code->push_back(asm_g);

                    DELETE(attr_e);
                }

                attr->type = new Type();
                attr->type->te = TE_VOID;
                attr->place = new string("");

                break;
            }

            int func_id = func_find(*f_name);

            if(func_id == -1) {
                attr_set_error(attr);
                sem_error(expr->line, "func not found");
            }

            Function* func = functions.at(func_id);

            if(expr->args->size() - 1 != func->args->size()) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect number of parameters");
                break;
            }

            size_t pushed = 0;

            // compute and push all params
            vector<Attr*> attr_ex;
            for(size_t i = 0; i < func->args->size(); i++) {
                Expression* arg_ex = expr->args->at(i+1)->val.eVal;
                attr_ex.push_back(compute(arg_ex, mem, attr));
            }

            for(size_t i = 0; i < func->args->size(); i++) {
                Symbol* arg_fu = func->args->at(i);

                // create temp for imm
                if(attr_ex[i]->place->at(0) == '#') {
                    string* place_prev = attr_ex[i]->place;

                    int tmp_id = mem_temp(mem, attr_ex[i]->type);

                    attr_ex[i]->place = sym_to_place(mem, tmp_id);

                    Attr* attr_mov = new Attr();
                    attr_mov->type = new Type(*(attr_ex[i]->type));
                    attr_mov->place = place_prev;

                    string* asm_g = asm_gen("mov", attr_mov, attr_ex[i]);
                    attr->code->push_back(asm_g);

                    DELETE(attr_mov);
                }

                // cast?
                if(type_eff(attr_ex[i]->type) != type_eff(arg_fu->type)) {
                    string* cast_c = cast(attr_ex[i], arg_fu->type, mem);
                    attr->code->push_back(cast_c);
                }

                attr_ref_addr(attr_ex[i]);

                string* asm_g = asm_gen("push", attr_ex[i]);
                attr->code->push_back(asm_g);

                pushed += 4;
                DELETE(attr_ex[i]);
            }
            attr_ex.clear();

            attr->type = new Type(*(func->result));

            // push temp for result
            if(func->result->te != TE_VOID) {
                int tmp_res_id = mem_temp(mem, func->result);
                Symbol* tmp_res = mem->vec->at(tmp_res_id);

                attr->place = sym_to_place(mem, tmp_res_id);

                Attr* attr_push = new Attr();
                attr_push->type = new Type(*(tmp_res->type));
                attr_push->place = new string("#" + *attr->place);

                string* asm_g = asm_gen("push", attr_push);
                attr->code->push_back(asm_g);

                pushed += 4;
                DELETE(attr_push);

            } else {
                attr->place = new string("");
            }

            Attr* attr_call = new Attr();
            attr_call->type = new Type();
            attr_call->type->te = TE_INTEGER;
            attr_call->place = new string("#" + *func->name);

            string* asm_g = asm_gen("call", attr_call);
            attr->code->push_back(asm_g);

            DELETE(attr_call);

            if(pushed != 0) {
                Attr* attr_inc = attr_imm(TE_INTEGER, "#" + to_string(pushed));

                string* asm_g = asm_gen("incsp", attr_inc);
                attr->code->push_back(asm_g);

                DELETE(attr_inc);
            }

        }
        break;

        /* ----------------------------------------------------------------- */

        case OP_LOG_NOT:
        case OP_LOG_NE:
        case OP_LOG_LE:
        case OP_LOG_GE:
        case OP_LOG_LO:
        case OP_LOG_GR:
        case OP_LOG_EQ:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Expression* arg_2 = nullptr;

            Attr* attr_1 = compute(arg_1, mem, attr);
            Attr* attr_2 = nullptr;

            Type* attr_1_type = new Type(*(attr_1->type));

            if(expr->oper != OP_LOG_NOT) {
                arg_2 = expr->args->at(1)->val.eVal;
                attr_2 = compute(arg_2, mem, attr);

            } else {
                attr_2 = attr_imm(TE_INTEGER, "#0");

                if(type_eff(attr_1->type) != TE_INTEGER) {
                    string* cast_c = cast(attr_1, attr_2->type, mem);
                    attr->code->push_back(cast_c);
                }
            }

            if(!type_is_num(attr_1->type) || !type_is_num(attr_2->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "incorrect logical operand");
            };

            string func;
            switch(expr->oper) {
                case OP_LOG_NOT:  { func = "je";  break; }
                case OP_LOG_NE:   { func = "jne"; break; }
                case OP_LOG_LE:   { func = "jle"; break; }
                case OP_LOG_GE:   { func = "jge"; break; }
                case OP_LOG_LO:   { func = "jl";  break; }
                case OP_LOG_GR:   { func = "jg";  break; }
                case OP_LOG_EQ:   { func = "je";  break; }
                default:          { func = "???"; break; }
            }

            Attr* temp_attr     = new Attr();
            temp_attr->type     = nullptr;

            if(expr->oper != OP_LOG_NOT) {
                temp_attr->type = new Type();
                temp_attr->type->te = TE_INTEGER;
            } else {
                temp_attr->type = new Type(*(attr_1_type));
            }

            int tmp_id          = mem_temp(mem, temp_attr->type);
            temp_attr->place    = sym_to_place(mem, tmp_id);

            string* lab1 = lab_next();
            string* lab2 = lab_next();

            Attr* lab1_attr  = attr_imm(TE_INTEGER, "#" + *lab1);
            Attr* lab2_attr  = attr_imm(TE_INTEGER, "#" + *lab2);
            Attr* attr_imm_0 = attr_imm(TE_INTEGER, "#0");
            Attr* attr_imm_1 = attr_imm(TE_INTEGER, "#1");

            string* asm_g = asm_gen(func, attr_1, attr_2, lab1_attr);
            attr->code->push_back(asm_g);

            asm_g = asm_gen("mov", attr_imm_0, temp_attr);
            attr->code->push_back(asm_g);

            asm_g = asm_gen("jump", lab2_attr);
            attr->code->push_back(asm_g);

            attr->code->push_back(new string(*lab1 + ":"));

            asm_g = asm_gen("mov", attr_imm_1, temp_attr);
            attr->code->push_back(asm_g);

            attr->code->push_back(new string(*lab2 + ":"));

            attr->place = sym_to_place(mem, tmp_id);

            if(expr->oper != OP_LOG_NOT) {
                attr->type = new Type(*(temp_attr->type));
            } else {
                attr->type = new Type(*(attr_1_type));
            }

            DELETE(attr_1);
            DELETE(attr_2);
            DELETE(attr_1_type);
            DELETE(temp_attr);
            DELETE(lab1);
            DELETE(lab2);
            DELETE(lab1_attr);
            DELETE(lab2_attr);
            DELETE(attr_imm_0);
            DELETE(attr_imm_1);
        }
        break;

        /* ----------------------------------------------------------------- */
    }

    if(parent != nullptr) {
        for(auto code : *(attr->code)) {
            parent->code->push_back(new string(*code));
        }
    }

    return attr;
}

/* --------------------------------------------------------------------------*/

Attr* compute(vector<Expression*>* v_expr, Memory* mem, Attr* parent) {
    Attr* attr = parent != nullptr ? parent : new Attr();

    attr->type = new Type();
    attr->type->te = TE_VOID;
    attr->place = new string("");

    for(auto expr: *v_expr) {
        Attr* c_attr = compute(expr, mem, attr);
        DELETE(c_attr);
    }

    return attr;
}

/* --------------------------------------------------------------------------*/

void attr_set_error(Attr* attr) {
    attr->place = new string("0");
    attr->type = new Type();
    attr->type->te = TE_ERROR;
}

/* --------------------------------------------------------------------------*/

void attr_ref_addr(Attr* attr) {
    if(attr->place->at(0) == '*') {
        attr->place->erase(0, 1);
    } else if(attr->place->at(0) == 'B') {
        attr->place->insert(0, "#");
        return;
    }

    if(attr->place->at(0) != 'B') {
        attr->place->insert(0, "#");
    }
}
/* --------------------------------------------------------------------------*/

Attr* attr_imm(TypeEnum te, string value) {
    Attr* attr  = new Attr();
    attr->type  = new Type();
    attr->type->te = te;
    attr->place = new string(value);
    return attr;
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

    if(sym->level == 1) {
        ss << "BP";

        if(sym->offset > 0) {
            ss << "+";
        }
    }

    ss << sym->offset;
    return new string(ss.str());
}

/* --------------------------------------------------------------------------*/

string* cast(Attr* attr, Type* type, Memory* mem) {
    string func = type_eff(type) == TE_REAL ? "inttoreal" : "realtoint";

    int tmp_id = mem_temp(mem, type);
    string* tmp_place = sym_to_place(mem, tmp_id);

    Attr* tmp_attr = new Attr();
    tmp_attr->place = new string(*tmp_place);
    string* code = asm_gen(func, attr, tmp_attr);

    DELETE(tmp_attr);
    DELETE(attr->place);
    DELETE(attr->type);

    attr->place = tmp_place;
    attr->type = new Type(*(mem->vec->at(tmp_id)->type));

    return code;
}

/* --------------------------------------------------------------------------*/

int lab_current = 1;
string* lab_next() {
    stringstream ss;
    ss << "lab" << to_string(lab_current);
    lab_current++;
    return new string(ss.str());
}

/* --------------------------------------------------------------------------*/

string* asm_gen(string command, Attr* arg1, Attr* arg2, Attr* arg3) {
    stringstream ss;

    ss << command << ".";

    bool real = type_eff(arg1->type) == TE_REAL && command != "push";
    ss << (real ? "r" : "i");

    ss << string(10-command.length(), ' ');
    ss << *arg1->place;

    if(arg2 != nullptr) {
        ss << "," << *(arg2->place);
    }

    if(arg3 != nullptr) {
        ss << "," << *(arg3->place);
    }

    return new string(ss.str());
}

/* ------------------------------------------------------------------------- */

void asm_gen_app() {
    cout << "\t" << "jump.i      #lab0" << "\n";

    for(auto func: functions) {
        auto attr = compute(func->body, func->stack);
        cout << *(func->name) << ":" << "\n";

        auto stack_v = func->stack->vec;
        auto alloc = max(-(stack_v->at(stack_v->size()-1)->offset), 0);
        cout << "\t" << "enter.i     #"  << alloc << "\n";

        for(auto code: *(attr->code)) {
            if(code->at(0) != 'l') cout << "\t";
            cout << *code << "\n";
        }
        DELETE(attr);

       cout << "\t" << "leave" << "\n";
       cout << "\t" << "return" << "\n";
    }

    cout << "lab0:" << "\n";
    auto attr = compute(&program, &memory);
    for(auto code: *(attr->code)) {
        if(code->at(0) != 'l') cout << "\t";
        cout << *code << "\n";
    }
    DELETE(attr);

    cout << "\t" << "exit" << "\n";

    if(errors.size() > 0) {
        cerr << "\n";
    }
    for(auto error: errors) {
        cerr << ";" <<*error << "\n";
        DELETE(error);
    }
    errors.clear();
}

/* --------------------------------------------------------------------------*/
