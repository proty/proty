#include <stdio.h>
#include <string.h>
#include <vm/opcodes.h>
#include "compiler.h"
#include "symtab.h"
#include "config.h"
#include "parser.h"

void Compiler_compileFile(const char* file) {
    void* scanner;
    Node* root;

    yylex_init(&scanner);
    yyset_in(stdin, scanner);
    int res = yyparse(scanner, &root);
    yylex_destroy(scanner);

    if (!res) {
        Context* context = calloc(sizeof(Context), 1);
        Compiler_compile(root, context);
        free(context);
    }
}

void Compiler_compileString(const char* str) {
    void* scanner;
    void* buf;
    Node* root;

    yylex_init(&scanner);
    buf = yy_scan_string(str, scanner);
    int res = yyparse(scanner, &root);
    yy_delete_buffer(buf, scanner);
    yylex_destroy(scanner);

    if (!res) {
        Context* context = calloc(sizeof(Context), 1);
        Compiler_compile(root, context);
        free(context);
    }
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
	printf("%s %i %i %i\n", node->data.sval, context->reg, lhs, rhs);
	return context->reg++;
    }

    case UnOpNode:
        printf("UnOp Node\n");
	Compiler_compile(node->right, context);
        return 0;

    case CallNode:
        printf("Call Node\n");
	return 0;

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
        printf("int %i %i\n", context->reg, node->data.ival);
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
