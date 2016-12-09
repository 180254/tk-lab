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
                string msg = "unknown variable " + *sym_name;
                sem_error(expr->line, msg.c_str());
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
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Expression* arg_2 = expr->args->at(1)->val.eVal;

            Attr* attr_1 = compute(arg_1, mem, attr);
            Attr* attr_2 = compute(arg_2, mem, attr);

            if(!type_is_num(attr_1->type) || !type_is_num(attr_2->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "assign operand is not number");
                break;
            }

            string hash = "#";
            if(startsWith(*(attr_1->place), hash)) {
                attr_set_error(attr);
                sem_error(expr->line, "cannot assign to imm value");
                break;
            }

            if(attr_1->type->te != attr_2->type->te) {
                string* cast_c = cast(attr_2, attr_1->type->te, mem);
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

        break;

        /* ----------------------------------------------------------------- */

        case OP_MATH_PLUS:
        case OP_MATH_MINUS:
        case OP_MATH_MUL:
        case OP_MATH_DIV1:
        {
            Expression* arg_1 = expr->args->at(0)->val.eVal;
            Expression* arg_2 = expr->args->at(1)->val.eVal;

            Attr* attr_1 = compute(arg_1, mem, attr);
            Attr* attr_2 = compute(arg_2, mem, attr);

            if(!type_is_num(attr_1->type) || !type_is_num(attr_2->type)) {
                attr_set_error(attr);
                sem_error(expr->line, "math operand is not number");
                break;
            }

            if(attr_1->type->te != attr_2->type->te) {
                Attr* cast_a = attr_1->type->te == TE_INTEGER ? attr_1 : attr_2;
                string* cast_c = cast(cast_a, TE_REAL, mem);
                attr->code->push_back(cast_c);
            }

            int tmp_id = mem_temp(mem, attr_1->type->te);
            attr->place = sym_to_place(mem, tmp_id);

            attr->type  = new Type();
            attr->type->te = attr_1->type->te;

            string func;
            switch(expr->oper) {
                case OP_MATH_PLUS:  { func = "add"; break; }
                case OP_MATH_MINUS: { func = "sub"; break; }
                case OP_MATH_MUL:   { func = "mul"; break; }
                case OP_MATH_DIV1:  { func = "sub"; break; }
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

        break;

        /* ----------------------------------------------------------------- */

        case OP_MATH_UPLUS:

        break;

        /* ----------------------------------------------------------------- */

        case OP_MATH_MOD:

        break;

        /* ----------------------------------------------------------------- */


        /* ----------------------------------------------------------------- */

        case OP_MATH_DIV2:

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

            if(*f_name == "write" || *f_name == "read") {
                for(size_t i = 1; i < expr->args->size(); i++) {
                    Expression* arg_e = expr->args->at(i)->val.eVal;
                    Attr* attr_e = compute(arg_e, mem, attr);

                    attr->type  = new Type();
                    attr->type->te = TE_VOID;
                    attr->place = new string("");

                    string* asm_g = asm_gen(*f_name, attr_e);
                    attr->code->push_back(asm_g);

                    DELETE(attr_e);
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
    attr->place = new string("");
    attr->type->te = TE_ERROR;
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
    string func = te == TE_INTEGER ? "realtoint" : "inttoreal";

    int tmp_id = mem_temp(mem, te);
    string* tmp_place = sym_to_place(mem, tmp_id);

    Attr* tmp_attr = new Attr();
    tmp_attr->place = new string(*tmp_place);

    string* code = asm_gen(func, attr, tmp_attr);

    DELETE(tmp_attr);
    DELETE(attr->place);

    attr->place = tmp_place;
    attr->type->te = te;

    return code;
}

/* --------------------------------------------------------------------------*/

int lab_current = 0;
string* lab_next() {
    stringstream ss;
    ss << "lab" << to_string(lab_current);
    return new string(ss.str());
}

/* --------------------------------------------------------------------------*/

string* asm_gen(string command, Attr* arg1, Attr* arg2, Attr* arg3) {
    stringstream ss;

    ss << command << "." << (arg1->type->te == TE_INTEGER ? "i" : "r");
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

        int alloc = 0;
        string s_temp = "$";
        for(auto sym: *(func->stack->vec)) {
            if(startsWith(*(sym->name), s_temp)) {
                alloc += type_size(sym->type);
            }
        }
        cout << "\t" << "enter       #"  << alloc << "\n";

        for(auto code: *(attr->code)) {
            cout << "\t" << *code << "\n";
        }
        DELETE(attr);

       cout << "\t" << "leave" << "\n";
       cout << "\t" << "return" << "\n";
    }

    cout << "lab0:" << "\n";
    auto attr = compute(&program, &memory);
    for(auto code: *(attr->code)) {
        cout << "\t" << *code << "\n";
    }
    DELETE(attr);

    cout << "\t" << "exit" << "\n";
}

/* --------------------------------------------------------------------------*/

bool startsWith(string& haystack, string& needle) {
    return needle.length() <= haystack.length()
        && equal(needle.begin(), needle.end(), haystack.begin());
}

/* --------------------------------------------------------------------------*/
