#include "node.h"

Node* Node_new(enum NodeTag tag, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->tag = tag;
    n->left = left;
    n->right = right;
    return n;
}
