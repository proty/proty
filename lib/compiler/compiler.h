#ifndef PROTY_COMPILER_H
#define PROTY_COMPILER_H

#include "ast.h"

typedef struct Context {
    int reg;
} Context;

void Compiler_compileFile(const char* file);
void Compiler_compileString(const char* string);
int Compiler_compile(Node* node, Context* context);

#endif
