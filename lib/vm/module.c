#include <stdlib.h>
#include <stdio.h>
#include "module.h"

Module* Module_new(const char* name) {
    Module* module = malloc(sizeof(Module));
    module->name = name;

    module->blocks = malloc(sizeof(Block*));
    module->blockc = 0;

    module->strings = malloc(sizeof(char*));
    module->symbols = malloc(sizeof(char*));

    module->symbolc = 0;
    module->stringc = 0;

    return module;
}

int Module_addBlock(Module* self, Block* block) {
    self->blocks = realloc(self->blocks, sizeof(char*)*(self->blockc+1));
    self->blocks[self->blockc] = block;
    return self->blockc++;
}

int Module_addSymbol(Module* self, const char* symbol) {
    self->symbols = realloc(self->symbols, sizeof(char*)*(self->symbolc+1));
    self->symbols[self->symbolc] = symbol;
    return self->symbolc++;
}

int Module_addString(Module* self, const char* string) {
    self->strings = realloc(self->strings, sizeof(char*)*(self->stringc+1));
    self->strings[self->stringc] = string;
    return self->stringc++;
}

void Module_dump(Module* self) {
    printf("Module %s\n", self->name);
    printf(":: %i Symbols\n", self->symbolc);
    for (int i = 0; i < self->symbolc; i++) {
      printf("%i | %s\n", i, self->symbols[i]);
    }

    printf(":: %i Strings\n", self->stringc);
    for (int i = 0; i < self->stringc; i++) {
      printf("%i | %s\n", i, self->strings[i]);
    }

    printf("~~~~~\n");

    for (int i = 0; i < self->blockc; i++) {
      Block_dump(self->blocks[i]);
    }
}
