#ifndef PROTY_COMPILER_H
#define PROTY_COMPILER_H

#include "node.h"
#include <vm/block.h>

typedef struct Context {
    Block* block;
    int reg;
} Context;

Block* Compiler_compileFile(const char* file);
Block* Compiler_compileString(const char* string);
Block* Compiler_compileRoot(Node* root, const char* name);
int Compiler_compile(Node* node, Context* context);

#endif
