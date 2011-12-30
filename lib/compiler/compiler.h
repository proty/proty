#ifndef PROTY_COMPILER_H
#define PROTY_COMPILER_H

typedef struct Module Module;
typedef struct Block Block;
typedef struct Node Node;
typedef struct SymTab SymTab;

/**
 * Holds the compilation context.
 */
typedef struct Context {
    /// the module
    Module* module;

    /// the current block
    Block* block;

    /// the symbol table
    SymTab* symtab;

    /// the register count
    int reg;
} Context;

/**
 * Creates a new compilation Context.
 * @param module The module.
 * @returns The new context;
 */
Context* Compiler_newContext(Module* module);

/**
 * Compiles a file to a Block.
 * @param context The compilation context.
 * @param file The file descriptor.
 * @returns The index of the compiled Block.
 */
int Compiler_compileFile(Context* context, FILE* file);

/**
 * Compiles a string to a Block.
 * @param context The compilation context.
 * @param string The string.
 * @returns The index of the compiled Block.
 */
int Compiler_compileString(Context* context, const char* string);

/**
 * Compiles a root node a Block.
 * @param context The compilation context.
 * @param root The root node.
 * @returns The index of the compiled Block.
 */
int Compiler_compileRoot(Context* context, Node* root);

/**
 * Compiles a node in the current context.
 * @param context The compilation context.
 * @param node The node.
 * @returns The register of the result.
 */
int Compiler_compile(Context* context, Node* node);

#endif
