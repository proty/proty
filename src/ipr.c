#include <stdlib.h>
#include <stdio.h>
#include <runtime/runtime.h>
#include <compiler/compiler.h>
#include <compiler/config.h>
#include <vm/eval.h>

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

    Context* context = Compiler_newContext();
    State* state = State_new();

    while (1) {
        char* input = readline(">>> ");

        if (!input) break;
        add_history(input);

        Block* block = Compiler_compileString(context, input);
        if (block) {
            Object* object = eval(state, block);
            printf("=> %i\n", object->data.i);
        }
        free(input);
    }

    free(context);
    return 0;
}
