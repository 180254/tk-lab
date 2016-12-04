#include "global.hpp"

/* -------------------------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    
    const char *app  = argv[0];
    const char *path = argv[1];
    
    if(argc < 2) {
        usage(app, "path parameter is misssing");
        return 1;
    }
    
    if(strcmp(path, "-") != 0) {
        yyin = fopen(path, "r");
        
        if(yyin == NULL) {
            usage(app, "given path is incorrect");
            return 2;
        }
    }
    
    yyparse();
    fclose(yyin);

    #if defined(CP_DEBUG) && CP_DEBUG
    mem_debug();
    #endif
    
    mem_free();

    return 0;
}


/* -------------------------------------------------------------------------------------------- */

void usage(const char *app, const char *message) {
    cerr << "ERROR: " << message << ".\n";
    cerr << "Usage: " << app << " PATH\n";
    cerr << "PATH = FILE-PATH | '-'\n";
}

