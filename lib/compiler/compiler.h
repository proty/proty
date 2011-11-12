#ifndef PROTY_COMPILER_H
#define PROTY_COMPILER_H

#include "node.h"
#include <vm/block.h>

/**
 * Holds the compilation context.
 */
typedef struct Context {
    /// the current block
    Block* block;

    /// the register count
    int reg;
} Context;

/**
 * Compiles a file to a Block.
 * @param file The filename.
 * @returns The compiled Block.
 */
Block* Compiler_compileFile(const char* file);

/**
 * Compiles a string to a Block.
 * @param string The string.
 * @returns The compiled Block.
 */
Block* Compiler_compileString(const char* string);

/**
 * Compiles a root node a Block.
 * @param root The root node.
 * @returns The compiled Block.
 */
Block* Compiler_compileRoot(Node* root);

/**
 * Compiles a node in the current context.
 * @param node The node.
 * @param context The compilation context.
 * @returns The register of the result.
 */
int Compiler_compile(Node* node, Context* context);

#endif
