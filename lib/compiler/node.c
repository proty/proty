#include "node.h"
#include <assert.h>

Node* Node_new(enum NodeTag tag, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->tag = tag;
    n->left = left;
    n->right = right;
    return n;
}

Node* AssignNode_new(Node* expr, Node* value) {
    Node* n = Node_new(AssignNode, 0, value);
    assert(expr->tag == NameNode);
    n->data.sval = expr->data.sval;
    free(expr);
    return n;
}

Node* BinOpNode_new(Node* lhs, Node* rhs, const char* op) {
    Node* n = Node_new(BinOpNode, lhs, rhs);
    n->data.node = Node_new(SymbolNode, 0, 0);
    n->data.node->data.sval = op;
    return n;
}

Node* GetSlotNode_new(Node* obj, const char* slot) {
    Node* n = Node_new(GetSlotNode, obj, 0);
    n->data.node = Node_new(SymbolNode, 0, 0);
    n->data.node->data.sval = slot;
    return n;
}

Node* SetSlotNode_new(Node* obj, Node* val, const char* slot) {
    Node* n = Node_new(SetSlotNode, obj, val);
    n->data.node = Node_new(SymbolNode, 0, 0);
    n->data.node->data.sval = slot;
    return n;
}

Node* SendNode_new(Node* obj, Node* args, const char* slot) {
    Node* n = Node_new(SendNode, obj, args);
    n->data.node = Node_new(SymbolNode, 0, 0);
    n->data.node->data.sval = slot;
    return n;
}
