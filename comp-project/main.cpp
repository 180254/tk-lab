#include "global.hpp"

/* -------------------------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    
    const char* app  = argv[0];
    const char* path = argv[1];
    
    if(argc < 2) {
        usage(app, "path parameter is misssing");
        return 1;
    }
    
    if(strcmp(path, "-") != 0) {
        yyin = fopen(path, "r");
        
        if(yyin == nullptr) {
            usage(app, "given path is incorrect");
            return 2;
        }
    }
    
    yyparse();
    fclose(yyin);

    #if defined(XC_DEBUG) && XC_DEBUG
    mem_debug();
    #endif
    
    mem_free();

    return 0;
}


/* -------------------------------------------------------------------------------------------- */

void usage(const char *app, const char *message) {
    cerr << "app error: " << message << "\n";
    cerr << "app usage: " << app << " PATH\n";
    cerr << "app usage: PATH = FILE-PATH | '-'\n";
}

