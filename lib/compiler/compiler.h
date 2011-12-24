#ifndef PROTY_COMPILER_H
#define PROTY_COMPILER_H

#include "node.h"
#include "symtab.h"
#include <vm/block.h>

/**
 * Holds the compilation context.
 */
typedef struct Context {
    /// the current block
    Block* block;

    /// the symbol table
    SymTab* symtab;

    /// the register count
    int reg;
} Context;

/**
 * Creates a new compilation Context.
 * @returns The new context;
 */
Context* Compiler_newContext();

/**
 * Compiles a file to a Block.
 * @param context The compilation context.
 * @param file The file descriptor.
 * @returns The compiled Block.
 */
Block* Compiler_compileFile(Context* context, FILE* file);

/**
 * Compiles a string to a Block.
 * @param context The compilation context.
 * @param string The string.
 * @returns The compiled Block.
 */
Block* Compiler_compileString(Context* context, const char* string);

/**
 * Compiles a root node a Block.
 * @param context The compilation context.
 * @param root The root node.
 * @returns The compiled Block.
 */
Block* Compiler_compileRoot(Context* context, Node* root);

/**
 * Compiles a node in the current context.
 * @param context The compilation context.
 * @param node The node.
 * @returns The register of the result.
 */
int Compiler_compile(Context* context, Node* node);

#endif
