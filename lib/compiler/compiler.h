#ifndef PROTY_COMPILER_H
#define PROTY_COMPILER_H

#include "ast.h"
#include <vm/module.h>
#include <vm/block.h>

typedef struct Context {
    Module* module;
    Block* block;
} Context;

Module* Compiler_compileFile(const char* file);
Module* Compiler_compileString(const char* string);
Module* Compiler_compileRoot(Node* root, const char* name);
void Compiler_compile(Node* node, Context* context);

#endif
