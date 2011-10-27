#include <stdio.h>
#include <string.h>
#include <vm/opcodes.h>
#include "compiler.h"
#include "symtab.h"
#include "config.h"
#include "parser.h"

Module* Compiler_compileFile(const char* file) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yyset_in(stdin, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    if (!res) {
        return Compiler_compileRoot(root, file);
    }
    else return 0;
}

Module* Compiler_compileString(const char* str) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yy_scan_string(str, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    if (!res) {
        return Compiler_compileRoot(root, "<eval>");
    }
    else return 0;
}

Module* Compiler_compileRoot(Node* root, const char* name) {
    Context* context = calloc(sizeof(Context), 1);

    context->module = Module_new(name);
    context->block = Block_new("entry");
    Module_addBlock(context->module, context->block);

    Compiler_compile(root, context);
    Block_append(context->block, OP_RET, 0);

    free(context);

    Module_dump(context->module);
    return context->module;
}

void Compiler_compile(Node* node, Context* context) {
    switch (node->tag) {
    case BranchNode:
        printf("Branch Node\n");
        Compiler_compile(node->left, context);
        break;

    case BinOpNode: {
        // push rhs onto the stack
        Compiler_compile(node->right, context);

        // push the symbol onto the stack
        int sym = Module_addSymbol(context->module, node->data.sval);
        Block_append(context->block, OP_SYM, 1, sym);

        // push lhs onto the stack
        Compiler_compile(node->left, context);

        // send the operator as message to lhs
        Block_append(context->block, OP_SEND, 1, 2);
    }

    case UnOpNode:
        printf("UnOp Node\n");
        Compiler_compile(node->right, context);
        break;

    case CallNode:
        printf("Call Node\n");
        break;

    case SendNode:
        printf("Send Node\n");
        break;

    case SetSlotNode:
        printf("SetSlotNode\n");
        break;

    case GetSlotNode:
        printf("GetSlotNode\n");
        break;

    case AssignNode:
        printf("Assign Node\n");
        break;

    case IfNode:
        printf("IF Node\n");
        break;

    case WhileNode:
        printf("WHILE Node\n");
        break;

    case IntegerNode:
        Block_append(context->block, OP_INT, 1, node->data.ival);
        break;

    case FloatNode:
        printf("Float Node\n");
        break;

    case StringNode:
        printf("String Node\n");
        break;

    case NameNode:
        printf("Name Node\n");
        break;

    case SymbolNode:
        printf("Symbol Node\n");
        break;

    default:
        printf("Something else %i\n", node->tag);
    }
}
