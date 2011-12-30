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
        ArgsNode,
        FuncArgsNode,
        SetSlotNode,
        GetSlotNode,
        AssignNode,
        IfNode,
        ElseNode,
        WhileNode,
        DoNode,
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
        struct Node* node;
    } data;

    struct Node* left;
    struct Node* right;
} Node;

Node* Node_new(enum NodeTag tag, Node* left, Node* right);
Node* AssignNode_new(Node* name, Node* value);
Node* BinOpNode_new(Node* lhs, Node* rhs, const char* op);
Node* GetSlotNode_new(Node* obj, const char* slot);
Node* SetSlotNode_new(Node* obj, Node* val, const char* slot);
Node* SendNode_new(Node* obj, Node* args, const char* slot);

#endif
