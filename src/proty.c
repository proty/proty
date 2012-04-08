#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <runtime/runtime.h>
#include <compiler/compiler.h>
#include <vm/module.h>
#include <vm/eval.h>

void version() {
    fprintf(stderr, "Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")\n");
    #ifdef GIT_VERSION
    fprintf(stderr, "[" GIT_VERSION "]\n");;
    #endif

    fprintf(stderr, "\n");
    fprintf(stderr, COPYRIGHT "\n");
    fprintf(stderr, "This program comes with ABSOLUTELY NO WARRANTY.\n");
    fprintf(stderr, "This is free software, and you are welcome to redistribute it\n");
    fprintf(stderr, "under certain conditions; see proty.cc/license for details.\n");
}

void help() {
    fprintf(stderr,
            "Usage: proty [options ...] [input file]\n"
            "  --help, -h   Shows this message\n"
            "  -c           Compile to bytecode\n"
            "  -o [file]    Set the output filename\n"
            "  --version    Shows version information\n"
            "  --           Stop reading options\n");
}

int main(int argc, const char** argv) {
    int compile = 0;
    int dump = 0;
    const char* filename = 0;
    const char* output = 0;

    for (int count = 1; count < argc; count++) {
        if (argv[count][0] == '-') {
            if      (!strcmp(argv[count], "-c"))          { compile = 1; }
            else if (!strcmp(argv[count], "-d"))          { dump = 1; }
            else if (!strcmp(argv[count], "-h"))          { help(); return 0; }
            else if (!strcmp(argv[count], "--help"))      { help(); return 0; }
            else if (!strcmp(argv[count], "-o"))          { output = argv[++count]; }
            else if (!strcmp(argv[count], "--version"))   { version(); return 0; }
            else if (!strcmp(argv[count], "--"))          { break; }
            else {
                help();
                fprintf(stderr, "proty: invalid option %s", argv[count]);
                return 1;
            }
        }
        else { filename = argv[count]; break; }
    }

    runtime_init();

    FILE* file = filename ? fopen(filename, "r") : stdin;

    if (!file) {
        fprintf(stderr, "proty: %s: no such file\n", filename);
        return 1;
    }

    Module* module;
    int blockId = 0;
    if (Module_probe(file)) {
        module = Module_read(file);
    }
    else {
        module = Module_new();
        Context* context = Compiler_newContext(module);
        blockId = Compiler_compileFile(context, file);
        Compiler_deleteContext(context);
        if (blockId < 0) return 1;
    }

    if (compile) {
        // generate the output filename if it's not specified
        if (!output) {
            // if we read from stdin, write to out.prc
            if (!filename) {
                output = "out.prc";
            }
            else {
                char* outf;
                int len = strlen(filename);

                // if the filename ends with .pr, replace with .prc
                // if not, append .prc
                if (!strcmp(filename + len - 3, ".pr")) {
                    outf = malloc(sizeof(char) * len + 2);
                    strncpy(outf, filename, len - 3);
                    outf[len - 3] = '\0';
                }
                else {
                    outf = malloc(sizeof(char) * len + 5);
                    strcpy(outf, filename);
                }

                strcat(outf, ".prc");
                output = (const char*)outf;
            }
        }

        FILE* out = fopen(output, "w");
        Module_write(module, out);
    }
    else if (dump) {
        Module_dump(module);
    }
    else {
        State* state = State_new(module);
        State_setGlobalState(state);
        eval(state, blockId);
        State_delete(state);
    }

    Module_delete(module);

    return 0;
}
