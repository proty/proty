#include <stdlib.h>
#include <stdio.h>
#include "module.h"

Module* Module_new(const char* name) {
    Module* module = malloc(sizeof(Module));
    module->name = name;

    module->blocks = malloc(sizeof(Block*));
    module->blockc = 0;

    return module;
}

int Module_addBlock(Module* self, Block* block) {
    self->blocks = realloc(self->blocks, sizeof(char*)*(self->blockc+1));
    self->blocks[self->blockc] = block;
    return self->blockc++;
}

void Module_dump(Module* self) {
    printf("Module %s\n", self->name);

    for (int i = 0; i < self->blockc; i++) {
      Block_dump(self->blocks[i]);
    }
}
