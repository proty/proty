#ifndef PROTY_MODULE_H
#define PROTY_MODULE_H

#include <stdio.h>
#include "block.h"

typedef struct Module {
    Block** blocks;
    int blockc;
} Module;

Module* Module_new();
void Module_delete(Module* self);

int Module_addBlock(Module* self, Block* block);

void Module_write(Module* self, FILE* file);
Module* Module_read(FILE* file);
int Module_probe(FILE* file);
void Module_dump(Module* self);

#endif
