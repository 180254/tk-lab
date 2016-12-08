#pragma once
using namespace std;

struct Attr {
    vector<string*>* code;
    string*          place;
    Type*            type;

    Attr();
};



Attr* compute(Expression*, vector<Symbol*>*);

void set_attr_error(Attr*);

string sym_to_place(Symbol*);
