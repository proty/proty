#include <stdio.h>
#include <string.h>
#include <vm/opcodes.h>
#include "compiler.h"
#include "symtab.h"
#include "config.h"
#include "parser.h"
#include "lexer.h"

Context* Compiler_newContext() {
    Context* context = malloc(sizeof(Context));

    context->reg = 0;
    context->block = 0;
    context->symtab = SymTab_new();

    return context;
}

Block* Compiler_compileFile(Context* context, const char* file) {
    void* scanner;
    Node* root;

    FILE* stream = fopen(file, "r");
    yylex_init(&scanner);
    yyset_in(stream, scanner);

    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    fclose(stream);

    return res ? 0 : Compiler_compileRoot(context, root);
}

Block* Compiler_compileString(Context* context, const char* str) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yy_scan_string(str, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    return res ? 0 : Compiler_compileRoot(context, root);
}

Block* Compiler_compileRoot(Context* context, Node* root) {
    context->block = Block_new();

    // return the last object
    int ret = Compiler_compile(context, root);
    Block_append(context->block, OP_RET, ret);

    Block* block = context->block;

    Block_dump(block);
    return block;
}

int Compiler_compile(Context* context, Node* node) {
    switch (node->tag) {
    case BranchNode:
        Compiler_compile(context, node->left);
        break;

    case BinOpNode: {
        int lhs = Compiler_compile(context, node->left);
        int rhs = Compiler_compile(context, node->right);
        int op = Compiler_compile(context, node->data.node);

        // push rhs onto the stack
        Block_append(context->block, OP_PUSH, rhs);

        // send the operator as message to lhs
        Block_append(context->block, OP_SEND, context->reg, lhs, op, 1);

        return context->reg++;
    }

    case UnOpNode:
        Compiler_compile(context, node->right);
        break;

    case CallNode: {
        int obj = Compiler_compile(context, node->left);
        int argc = Compiler_compile(context, node->right);

        Block_append(context->block, OP_CALL, context->reg, obj, argc);
        return context->reg++;
    }

    case SendNode: {
        int obj = Compiler_compile(context, node->left);
        int argc = Compiler_compile(context, node->right);
        int slot = Compiler_compile(context, node->data.node);

        Block_append(context->block, OP_SEND, context->reg, obj, slot, argc);
        return context->reg++;
    }

    case ArgsNode: {
        int argc = 0;
        Node* args = node;
        while (args) {
            argc++;
            int arg = Compiler_compile(context, args->left);
            Block_append(context->block, OP_PUSH, arg);
            args = args->right;
        };
        return argc;
    }

    case SetSlotNode: {
        int obj = Compiler_compile(context, node->left);
        int val = Compiler_compile(context, node->right);
        int slot = Compiler_compile(context, node->data.node);
        Block_append(context->block, OP_SET, context->reg, obj, slot, val);
        return context->reg++;
    }

    case GetSlotNode: {
        int obj = Compiler_compile(context, node->left);
        int slot = Compiler_compile(context, node->data.node);
        Block_append(context->block, OP_GET, context->reg, obj, slot);
        return context->reg++;
    }

    case AssignNode: {
        int reg = SymTab_lookup(context->symtab, node->data.sval);
        int obj = Compiler_compile(context, node->right);

        if (!reg) {
            SymTab_store(context->symtab, node->data.sval, context->reg);
            reg = context->reg++;
        }

        Block_append(context->block, OP_MOV, reg, obj);
        return reg;
    }

    case IfNode:
        printf("IF Node\n");
        break;

    case WhileNode: {
        int start = context->block->size;

        int cond = Compiler_compile(context, node->left);
        int cond_jmp = context->block->size+1;

        Block_append(context->block, OP_JNS, cond);
        Compiler_compile(context, node->right);

        int diff = start - context->block->size - 1;
        Block_append(context->block, OP_JMP, diff);

        int end = context->block->size+1;
        Block_replace(context->block, cond_jmp, end);

        return cond;
    }

    case IntegerNode:
        Block_append(context->block, OP_INT, context->reg, node->data.ival);
        return context->reg++;

    case FloatNode: {
        double* data = malloc(sizeof(double));
        *data = node->data.dval;
        int fl = Block_const(context->block, CONST_FLOAT, (void*)data);
        Block_append(context->block, OP_FLOAT, context->reg, fl);
        return context->reg++;
    }

    case StringNode: {
        int str = Block_const(context->block, CONST_STR, (void*)node->data.sval);
        Block_append(context->block, OP_STR, context->reg, str);
        return context->reg++;
    }

    case NameNode: {
        int reg = SymTab_lookup(context->symtab, node->data.sval);
        if (!reg) {
            printf("name %s not defined\n", node->data.sval);
            abort();
        }
        return reg;
    }

    case SymbolNode: {
        int sym = Block_const(context->block, CONST_SYM, (void*)node->data.sval);
        Block_append(context->block, OP_SYM, context->reg, sym);
        return context->reg++;
    }

    default:
        abort();
    }
}
