#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vm/module.h>
#include <vm/opcodes.h>
#include <vm/state.h>
#include "node.h"
#include "compiler.h"
#include "symtab.h"
#include "parser.h"
#include "lexer.h"

Context* Compiler_newContext(Module* module) {
    Context* context = malloc(sizeof(Context));

    context->reg = 0;
    context->block = 0;
    context->module = module;
    context->symtab = SymTab_new();

    return context;
}

int Compiler_compileFile(Context* context, FILE* file) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yyset_in(file, scanner);

    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    return res ? -1 : Compiler_compileRoot(context, root);
}

int Compiler_compileString(Context* context, const char* str) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yy_scan_string(str, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    return res ? -1 : Compiler_compileRoot(context, root);
}

int Compiler_compileRoot(Context* context, Node* root) {
    context->block = Block_new();
    int id = Module_addBlock(context->module, context->block);

    if (root) {
        int ret = Compiler_compile(context, root);
        Block_append(context->block, OP_RET, ret);
    }
    else {
        Block_append(context->block, OP_NIL, context->reg);
        Block_append(context->block, OP_RET, context->reg++);
    }

    return id;
}

int Compiler_compile(Context* context, Node* node) {
    switch (node->tag) {
    case BranchNode: {
        Reg left = Compiler_compile(context, node->left);
        if (node->right) {
            return Compiler_compile(context, node->right);
        }
        else {
            return left;
        }
    }

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
        return 0;

    case CallNode: {
        if (node->left->tag == NameNode) {
            if (!strcmp(node->left->data.sval, "load")) {
                assert(node->right->left->tag == StringNode);
                const char* name = node->right->left->data.sval;
                int str = Block_const(context->block, CONST_STR, (void*)name);
                Block_append(context->block, OP_LOAD, context->reg, str);
                return context->reg++;
            }
        }

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

    case DoNode: {
        // backup current context
        Block* oldBlock = context->block;
        SymTab* oldSymtab = context->symtab;

        // create context for the new function
        context->block = Block_new();
        context->symtab = SymTab_new();

        // pop the arguments from the stack
        // and save the registers to the symtab
        int argc = 0;
        Node* arg = node->left;
        while (arg) {
            Block_append(context->block, OP_POP, context->reg);
            SymTab_store(context->symtab, arg->data.sval, context->reg++);
            arg = arg->right;
            argc++;
        }

        // compile the function code
        int ret = Compiler_compile(context, node->right);
        Block_append(context->block, OP_RET, ret);
        int block = Module_addBlock(context->module, context->block);

        // restore the old context
        SymTab_delete(context->symtab);
        context->symtab = oldSymtab;
        context->block = oldBlock;

        // create the function
        Block_append(context->block, OP_FUN, context->reg, block, argc);
        return context->reg++;
    }

    case ArgsNode: {
        int argc = 0;
        Node* args = node;
        while (args) {
            if (args->left) {
                argc++;
                int arg = Compiler_compile(context, args->left);
                Block_append(context->block, OP_PUSH, arg);
            }
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

        if (reg < 0) {
            SymTab_store(context->symtab, node->data.sval, context->reg);
            reg = context->reg++;
        }

        Block_append(context->block, OP_MOV, reg, obj);
        return reg;
    }

    case IfNode: {
        int diff;
        Reg res = context->reg++;

        // compile condition
        Reg cond = Compiler_compile(context, node->left);
        int cond_jmp = Block_append(context->block, OP_JNS, cond, 0) + 2;

        if (node->right->tag == ElseNode) {
            Reg res_a = Compiler_compile(context, node->right->left);
            Block_append(context->block, OP_MOV, res, res_a);
        }
        else {
            Reg res_a = Compiler_compile(context, node->right);
            Block_append(context->block, OP_MOV, res, res_a);
        }

        // add jump to the end
        int end_jmp = Block_append(context->block, OP_JMP, 0) + 1;

        // change condition jump to this position
        diff = Block_position(context->block) - cond_jmp + 1;
        Block_replace(context->block, cond_jmp, diff);

        if (node->right->tag == ElseNode) {
            Reg res_b = Compiler_compile(context, node->right->right);
            Block_append(context->block, OP_MOV, res, res_b);
        }
        else {
            Block_append(context->block, OP_NIL, res);
        }

        // change end jump to this position
        diff = Block_position(context->block) - end_jmp;
        Block_replace(context->block, end_jmp, diff);

        return res;
    }

    case WhileNode: {
        int start = Block_position(context->block) - 1;
        Reg cond = Compiler_compile(context, node->left);

        int cond_jmp = Block_append(context->block, OP_JNS, cond, 0) + 2;
        Compiler_compile(context, node->right);

        int diff = start - Block_position(context->block);
        int end = Block_append(context->block, OP_JMP, diff);
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
        char* str = (char*)node->data.sval;

        // replace control characters
        int i = 0;
        int off = 0;
        while(str[i+off] != '\0') {
            if (str[i+off] == '\\') {
                char c = str[i+off+1];
                if      (c == 'a') str[i] = '\a';
                else if (c == 'b') str[i] = '\b';
                else if (c == 'f') str[i] = '\f';
                else if (c == 'n') str[i] = '\n';
                else if (c == 'r') str[i] = '\r';
                else if (c == 't') str[i] = '\t';
                else if (c == 'v') str[i] = '\v';
                else if (c == 'x') {
                    char hex[3];
                    unsigned int val;

                    hex[0] = str[i+off+2];
                    hex[1] = str[i+off+3];
                    hex[2] = '\0';
                    sscanf(hex, "%x", &val);

                    str[i] = (char)val;
                    off += 2;
                }
                else off--;
                off++;
            }
            i++;
            if (off) str[i] = str[i+off];
        }

        int str_const = Block_const(context->block, CONST_STR, (void*)str);
        Block_append(context->block, OP_STR, context->reg, str_const);
        return context->reg++;
    }

    case NameNode: {
        Reg reg = SymTab_lookup(context->symtab, node->data.sval);

        if (reg < 0) {
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
        return 0;
    }
}
