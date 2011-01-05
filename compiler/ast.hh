#ifndef PROTY_AST_HH
#define PROTY_AST_HH

#include <iostream>
#include "llvm.hh"

class Compiler;

class Node {
public:
  virtual llvm::Value* codegen(Compiler*) = 0;
};

class BlockNode : public Node {
private:
  std::vector<Node*> nodes;

public:
  void add(Node* n) { nodes.push_back(n); }
  llvm::Value* codegen(Compiler*);
};

class IntegerNode : public Node {
private:
  int value;

public:
  IntegerNode(int value) : value(value) {}

  llvm::Value* codegen(Compiler*);
};

class StringNode : public Node {
private:
  std::string value;

public:
  StringNode(std::string value) : value(value) {}

  llvm::Value* codegen(Compiler*);
};

class BinaryOpNode : public Node {
private:
  std::string op;
  Node* lhs;
  Node* rhs;

public:
  BinaryOpNode(std::string op, Node* lhs, Node* rhs)
    : op(op), lhs(lhs), rhs(rhs) {}

  llvm::Value* codegen(Compiler*);
};

class NameNode : public Node {
private:
  std::string value;

public:
  NameNode(std::string value) : value(value) {}

  llvm::Value* codegen(Compiler*);
};

#endif
