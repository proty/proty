#include <stdlib.h>
#include <stdio.h>
#include <runtime/runtime.h>
#include <compiler/compiler.h>
#include <vm/module.h>
#include <vm/eval.h>
#include <io/io.h>

// GNU readline
#include <readline/readline.h>
#include <readline/history.h>

int main(int argc, char** argv) {
    printf("Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")\n");
    #ifdef GIT_VERSION
    printf("[" GIT_VERSION "]\n");
    #endif
    printf("\n");

    runtime_init();

    Module* module = Module_new();
    Context* context = Compiler_newContext(module);
    State* state = State_new(module);

    while (1) {
        char* input = readline(">>> ");

        if (!input) break;
        add_history(input);

        int block = Compiler_compileString(context, input);

        if (block >= 0) {
            Object* object = eval(state, block);

            printf("=> ");
            io_print(0, object);
        }
        free(input);
    }

    free(context);
    State_delete(state);

    return 0;
}
