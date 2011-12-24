#include <stdio.h>
#include <string.h>
#include <runtime/runtime.h>
#include <compiler/compiler.h>
#include <compiler/config.h>
#include <vm/eval.h>
#include <vm/module.h>

void version() {
    fprintf(stderr, "Proty " VERSION " " BUILD_TYPE " (" __DATE__ ", " __TIME__ ")\n");
    #ifdef GIT_VERSION
    fprintf(stderr, "[" GIT_VERSION "]\n");;
    #endif
    fprintf(stderr, "\n");
    fprintf(stderr, "This program comes with ABSOLUTELY NO WARRANTY.\n");
    fprintf(stderr, "This is free software, and you are welcome to redistribute it\n");
    fprintf(stderr, "under certain conditions; see proty.cc/license for details.\n");
}

void help() {
    fprintf(stderr,
            "Usage: proty [options ...] [input file]\n"
            "  --help, -h   Shows this message\n"
            "  -o [file]    Set output file\n"
            "  --version    Shows version information\n"
            "  --           Stop reading options\n");
}

int main(int argc, const char** argv) {
  const char* filename = 0;
  const char* output = 0;

  for (int count = 1; count < argc; count++) {
      if (argv[count][0] == '-') {
          if (!strcmp(argv[count], "-h"))               { help(); return 0; }
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
  Module* module;

  if (Module_probe(file)) {
      module = Module_read(file);
  }
  else {
      Context* context = Compiler_newContext();
      module = Module_new();

      Block* block = Compiler_compileFile(context, file);
      Module_addBlock(module, block);
      free(context);
  }

  State* state = State_new();
  eval(state, module->blocks[0]);
  Block_dump(module->blocks[0]);

  if (output) {
      FILE* out = fopen(output, "w");
      Module_write(module, out);
  }

  Module_delete(module);
  State_delete(state);

  return 0;
}
