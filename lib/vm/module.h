#ifndef PROTY_MODULE_H
#define PROTY_MODULE_H

#include "block.h"

typedef struct Module {
    const char* name;

    Block** blocks;
    int blockc;

    const char** symbols;
    const char** strings;
    int symbolc;
    int stringc;
} Module;

Module* Module_new(const char* name);
int Module_addBlock(Module* self, Block* block);
int Module_addSymbol(Module* self, const char* symbol);
int Module_addString(Module* self, const char* string);
void Module_dump(Module* self);

#endif
