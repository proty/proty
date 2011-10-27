#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <compiler/compiler.h>
#include <vm/eval.h>
#include <compiler/config.h>

int main(int argc, char** argv) {
    printf("Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")\n");
#ifdef GIT_VERSION
    printf("[" GIT_VERSION "]\n");
#endif
    printf("\n");

    while (1) {
        char* input = readline(">>> ");

        if (!input) break;
        add_history(input);

        Module* m = Compiler_compileString(input);
        Object* o = eval(m);
        free(input);
    }
    return 0;
}
