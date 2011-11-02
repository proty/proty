#ifndef PROTY_MODULE_H
#define PROTY_MODULE_H

#include "block.h"

typedef struct Module {
    const char* name;

    Block** blocks;
    int blockc;
} Module;

Module* Module_new(const char* name);
int Module_addBlock(Module* self, Block* block);
void Module_dump(Module* self);

#endif
