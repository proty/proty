#include <stdio.h>
#include <string.h>
#include <vm/opcodes.h>
#include "compiler.h"
#include "symtab.h"
#include "config.h"
#include "parser.h"
#include "lexer.h"

Block* Compiler_compileFile(const char* file) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yyset_in(stdin, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    return res ? 0 : Compiler_compileRoot(root, file);
}

Block* Compiler_compileString(const char* str) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yy_scan_string(str, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    return res ? 0 : Compiler_compileRoot(root, "<eval>");
}

Block* Compiler_compileRoot(Node* root, const char* name) {
    Context* context = calloc(sizeof(Context), 1);

    context->reg = 0;
    context->block = Block_new();

    // return the last object
    int ret = Compiler_compile(root, context);
    Block_append(context->block, OP_RET, ret);

    Block* block = context->block;
    free(context);

    return block;
}

int Compiler_compile(Node* node, Context* context) {
    switch (node->tag) {
    case BranchNode:
        printf("Branch Node\n");
        Compiler_compile(node->left, context);
        break;

    case BinOpNode: {
        int lhs = Compiler_compile(node->left, context);
        int rhs = Compiler_compile(node->right, context);

        // get the operator symbol
        int sym = Block_constant(context->block, CONSTANT_SYMBOL, (void*)node->data.sval);
        Block_append(context->block, OP_SYM, context->reg, sym);
        int op = context->reg++;

        // push rhs onto the stack
        Block_append(context->block, OP_PUSH, rhs);

        // send the operator as message to lhs
        Block_append(context->block, OP_SEND, context->reg, lhs, op, 1);

        return context->reg++;
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
        Block_append(context->block, OP_INT, context->reg, node->data.ival);
        return context->reg++;

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
