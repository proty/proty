#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vm/opcodes.h>
#include <vm/state.h>
#include "compiler.h"
#include "parser.h"
#include "lexer.h"

int yyparse(void* scanner, Node** root);

Context* Compiler_newContext(Module* module) {
    Context* context = malloc(sizeof(Context));

    context->reg = 0;
    context->block = 0;
    context->module = module;
    context->symtab = SymTab_new();

    return context;
}

void Compiler_deleteContext(Context* context) {
    SymTab_delete(context->symtab);
    free(context);
}

int Compiler_compileFile(Context* context, FILE* file) {
    void* scanner;
    Node* root = 0;

    yylex_init(&scanner);
    yyset_in(file, scanner);

    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);
    if (res) return -1;

    res = Compiler_compileRootNode(context, root);
    if (root) Node_delete(root);
    return res;
}

int Compiler_compileString(Context* context, const char* str) {
    void* scanner;
    Node* root = 0;

    yylex_init(&scanner);
    yy_scan_string(str, scanner);

    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);
    if (res) return -1;

    res = Compiler_compileRootNode(context, root);
    if (root) Node_delete(root);
    return res;
}

int Compiler_compileRootNode(Context* context, Node* root) {
    context->block = Block_new();
    int id = Module_addBlock(context->module, context->block);

    // this code is only executed if an error occurs in the
    // compilation process further down
    if (setjmp(context->error_buf)) {
        return -1;
    }

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

static void Compiler_error(Context* context) {
    longjmp(context->error_buf, 1);
}

static int Compiler_compileBranchNode(Context* context, Node* node) {
    Reg ret;

    if (node->left) {
        ret = Compiler_compile(context, node->left);
    }
    if (node->right) {
        ret = Compiler_compile(context, node->right);
    }

    return ret;
}

static int Compiler_compileBinOpNode(Context* context, Node* node) {
    int lhs = Compiler_compile(context, node->left);
    int rhs = Compiler_compile(context, node->right);
    int op = Compiler_compile(context, node->data.node);

    // push rhs onto the stack
    Block_append(context->block, OP_PUSH, rhs);

    // send the operator as message to lhs
    Block_append(context->block, OP_SEND, context->reg, lhs, op, 1);

    return context->reg++;
}

static int Compiler_compileUnOpNode(Context* context, Node* node) {
    int obj = Compiler_compile(context, node->right);
    int op = Compiler_compile(context, node->data.node);

    Block_append(context->block, OP_SEND, context->reg, obj, op, 0);
    return context->reg++;
}

static int Compiler_compileCallNode(Context* context, Node* node) {
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

static int Compiler_compileSendNode(Context* context, Node* node) {
    int obj = Compiler_compile(context, node->left);
    int argc = Compiler_compile(context, node->right);
    int slot = Compiler_compile(context, node->data.node);

    Block_append(context->block, OP_SEND, context->reg, obj, slot, argc);
    return context->reg++;
}

static int Compiler_compileDoNode(Context* context, Node* node) {
    // backup current block
    Block* oldBlock = context->block;

    // create context for the new function
    context->block = Block_new();
    SymTab_enterScope(context->symtab);

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
    SymTab_leaveScope(context->symtab);
    context->block = oldBlock;

    // create the function
    Block_append(context->block, OP_FUN, context->reg, block, argc);
    return context->reg++;
}

static int Compiler_compileArgsNode(Context* context, Node* node) {
    int argc;

    if (node->right) {
        argc = Compiler_compile(context, node->right) + 1;
    }
    else argc = 1;

    if (node->left) {
        int arg = Compiler_compile(context, node->left);
        Block_append(context->block, OP_PUSH, arg);
    }
    else if (!node->right) argc = 0;

    return argc;
}

static int Compiler_compileSetSlotNode(Context* context, Node* node) {
    int obj = Compiler_compile(context, node->left);
    int val = Compiler_compile(context, node->right);
    int slot = Compiler_compile(context, node->data.node);
    Block_append(context->block, OP_SET, context->reg, obj, slot, val);
    return context->reg++;
}

static int Compiler_compileGetSlotNode(Context* context, Node* node) {
    int obj = Compiler_compile(context, node->left);
    int slot = Compiler_compile(context, node->data.node);
    Block_append(context->block, OP_GET, context->reg, obj, slot);
    return context->reg++;
}

static int Compiler_compileAssignNode(Context* context, Node* node) {
    int reg = SymTab_lookup(context->symtab, node->data.sval);
    int obj = Compiler_compile(context, node->right);

    if (reg < 0) {
        SymTab_store(context->symtab, node->data.sval, context->reg);
        reg = context->reg++;
    }

    Block_append(context->block, OP_MOV, reg, obj);
    return reg;
}

static int Compiler_compileSubscriptNode(Context* context, Node* node) {
    int obj = Compiler_compile(context, node->left);
    int sub = Compiler_compile(context, node->right);

    // generate [] symbol
    Node* sym = Node_new(SymbolNode, 0, 0);
    sym->data.sval = "[]";
    int op = Compiler_compile(context, sym);

    Block_append(context->block, OP_PUSH, sub);
    Block_append(context->block, OP_SEND, context->reg, obj, op, 1);

    return context->reg++;
}

static int Compiler_compileIfNode(Context* context, Node* node) {
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

static int Compiler_compileWhileNode(Context* context, Node* node) {
    int start = Block_position(context->block) - 1;
    Reg cond = Compiler_compile(context, node->left);

    int cond_jmp = Block_append(context->block, OP_JNS, cond, 0) + 2;
    Compiler_compile(context, node->right);

    int diff = start - Block_position(context->block);
    int end = Block_append(context->block, OP_JMP, diff);
    Block_replace(context->block, cond_jmp, end);

    return cond;
}

static int Compiler_compileTryNode(Context* context, Node* node) {
    int catch_jmp = Block_append(context->block, OP_TRY, 0) + 1;
    int ret = Compiler_compile(context, node->left);
    Block_append(context->block, OP_ENDTRY);

    int skip_catch = Block_append(context->block, OP_JMP, 0) + 1;

    int diff;
    diff = Block_position(context->block) - catch_jmp;
    Block_replace(context->block, catch_jmp, diff);

    Compiler_compile(context, node->right);

    diff = Block_position(context->block) - skip_catch;
    Block_replace(context->block, skip_catch, diff);

    return ret;
}

static int Compiler_compileListNode(Context* context, Node* node) {
    int argc = Compiler_compile(context, node->left);
    Block_append(context->block, OP_LIST, context->reg, argc);
    return context->reg++;
}

static int Compiler_compileHashNode(Context* context, Node* node) {
    int argc = Compiler_compile(context, node->left);
    Block_append(context->block, OP_HASH, context->reg, argc);
    return context->reg++;
}

static int Compiler_compileHashArgsNode(Context* context, Node* node) {
    int argc;

    if (node->right) {
        argc = Compiler_compile(context, node->right) + 1;
    }
    else argc = 1;

    if (node->left) {
        assert(node->left->tag == HashElementNode);

        int key = Compiler_compile(context, node->left->left);
        int obj = Compiler_compile(context, node->left->right);

        Block_append(context->block, OP_PUSH, key);
        Block_append(context->block, OP_PUSH, obj);
    }
    else if (!node->right) argc = 0;

    return argc;
}

static int Compiler_compileIntegerNode(Context* context, Node* node) {
    Block_append(context->block, OP_INT, context->reg, node->data.ival);
    return context->reg++;
}

static int Compiler_compileFloatNode(Context* context, Node* node) {
    double* data = malloc(sizeof(double));
    *data = node->data.dval;
    int fl = Block_const(context->block, CONST_FLOAT, (void*)data);
    Block_append(context->block, OP_FLOAT, context->reg, fl);
    return context->reg++;
}

static int Compiler_compileStringNode(Context* context, Node* node) {
    char* str = (char*)node->data.sval;

    // replace escape sequences
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

static int Compiler_compileNameNode(Context* context, Node* node) {
    const char* name = node->data.sval;

    if (!strcmp(name, "true")) {
        Block_append(context->block, OP_BOOL, context->reg, 1);
        return context->reg++;
    }
    else if (!strcmp(name, "false")) {
        Block_append(context->block, OP_BOOL, context->reg, 0);
        return context->reg++;
    }
    else if (!strcmp(name, "nil")) {
        Block_append(context->block, OP_NIL, context->reg);
        return context->reg++;
    }

    Reg reg = SymTab_lookup(context->symtab, name);

    if (reg < 0) {
        fprintf(stderr, "name %s not defined\n", name);
        Compiler_error(context);
    }

    return reg;
}

static int Compiler_compileSymbolNode(Context* context, Node* node) {
    int sym = Block_const(context->block, CONST_SYM, (void*)node->data.sval);
    Block_append(context->block, OP_SYM, context->reg, sym);
    return context->reg++;
}

int Compiler_compile(Context* context, Node* node) {
    // dispatch!
    switch (node->tag) {
    case BranchNode:   return Compiler_compileBranchNode(context, node);
    case BinOpNode:    return Compiler_compileBinOpNode(context, node);
    case UnOpNode:     return Compiler_compileUnOpNode(context, node);
    case CallNode:     return Compiler_compileCallNode(context, node);
    case SendNode:     return Compiler_compileSendNode(context, node);
    case DoNode:       return Compiler_compileDoNode(context, node);
    case ArgsNode:     return Compiler_compileArgsNode(context, node);
    case SetSlotNode:  return Compiler_compileSetSlotNode(context, node);
    case GetSlotNode:  return Compiler_compileGetSlotNode(context, node);
    case AssignNode:   return Compiler_compileAssignNode(context, node);
    case SubscriptNode:return Compiler_compileSubscriptNode(context, node);
    case IfNode:       return Compiler_compileIfNode(context, node);
    case WhileNode:    return Compiler_compileWhileNode(context, node);
    case TryNode:      return Compiler_compileTryNode(context, node);
    case ListNode:     return Compiler_compileListNode(context, node);
    case HashNode:     return Compiler_compileHashNode(context, node);
    case HashArgsNode: return Compiler_compileHashArgsNode(context, node);
    case IntegerNode:  return Compiler_compileIntegerNode(context, node);
    case FloatNode:    return Compiler_compileFloatNode(context, node);
    case StringNode:   return Compiler_compileStringNode(context, node);
    case NameNode:     return Compiler_compileNameNode(context, node);
    case SymbolNode:   return Compiler_compileSymbolNode(context, node);
    default:
        abort();
        return 0;
    }
}
