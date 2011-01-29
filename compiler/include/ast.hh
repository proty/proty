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

  std::string getValue() { return value; }
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

class CallNode : public Node {
private:
  Node* callee;
  std::vector<Node*> args;

public:
  CallNode(Node* callee) : callee(callee) {}

  void addArg(Node* n) { args.push_back(n); }
  llvm::Value* codegen(Compiler*);
};

class FunctionNode : public Node {
private:
 Node* block;
  std::vector<std::string> args;

public:
  FunctionNode(Node* block=0) : block(block) {}

  void addArg(std::string arg) { args.push_back(arg); }
  void setBlock(Node* block) { this->block = block; }
  llvm::Value* codegen(Compiler*);
};


class NameNode : public Node {
private:
  std::string value;

public:
  NameNode(std::string value) : value(value) {}

  std::string getValue() { return value; }
  llvm::Value* codegen(Compiler*);
};

#endif
