#include "global.h"

int main(int, char *[]);
void usage(const char *, const char *);

/* -------------------------------------------------------------------------------------------- */

int main(int argc, char *argv[]) {
    
    const char *app = argv[0];
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
    
     //memory dump
     std::cout << "X\n";
    for(auto mem_ptr : memory) {
        std::cout << (*mem_ptr).to_string() << "\n";
    }
    return 0;
}


/* -------------------------------------------------------------------------------------------- */

void usage(const char *app, const char *message) {
    fprintf(stderr, "ERROR: %s.\n", message);
    fprintf(stderr, "Usage: %s PATH\n", app);
    fprintf(stderr, "PATH = FILE-PATH | '-'\n");
}

