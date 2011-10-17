#ifndef PROTY_AST_H
#define PROTY_AST_H

#include <stdlib.h>

typedef struct Node {
    enum NodeTag {
        BranchNode,
        BinOpNode,
        UnOpNode,
        CallNode,
        SendNode,
        SetSlotNode,
        GetSlotNode,
        AssignNode,
        IfNode,
        ElseNode,
        WhileNode,
        IntegerNode,
        FloatNode,
        StringNode,
        NameNode,
        SymbolNode,
    } tag;

    union {
        int ival;
        double dval;
        const char* sval;
    } data;

    struct Node* left;
    struct Node* right;

    int lineno;
} Node;

Node* Node_new(enum NodeTag tag, Node* left, Node* right);

#endif
