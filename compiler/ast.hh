#ifndef PROTY_AST_HH
#define PROTY_AST_HH

#include <iostream>
#include "llvm.hh"

class Compiler;

class Node {
public:
  virtual llvm::Value* codegen(Compiler*) = 0;
};

/**
 * A code block.
 * $nodes end | { $nodes }
 */

class BlockNode : public Node {
private:
  std::vector<Node*> nodes;

public:
  void add(Node* n) { nodes.push_back(n); }
  llvm::Value* codegen(Compiler*);
};


/**
 * An integer object.
 */

class IntegerNode : public Node {
private:
  int value;

public:
  IntegerNode(int value) : value(value) {}

  llvm::Value* codegen(Compiler*);
};


/**
 * Reference to bool object.
 * true|false
 */

class BoolNode : public Node {
private:
  bool value;

public:
  BoolNode(bool value) : value(value) {}

  llvm::Value* codegen(Compiler*);
};


/**
 * Reference to nil object.
 */

class NilNode : public Node {
public:
  NilNode() {}

  llvm::Value* codegen(Compiler*);
};


/**
 * A string object.
 * "$value"
 */

class StringNode : public Node {
private:
  std::string value;

public:
  StringNode(std::string value) : value(value) {}

  std::string getValue() { return value; }
  llvm::Value* codegen(Compiler*);
};


/**
 * A binary operation.
 * $lhs $op $rhs
 */

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


/**
 * A function call.
 * $callee($args)
 */

class CallNode : public Node {
private:
  Node* callee;
  std::vector<Node*> args;

public:
  CallNode(Node* callee) : callee(callee) {}

  void addArg(Node* n) { args.push_back(n); }
  llvm::Value* codegen(Compiler*);
};


/**
 * A function object.
 * do ($args) $block
 */

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


/**
 * An if statement.
 *
 * if $cond
 *   $thenNode
 * else
 *   $elseNode
 * end
 */

class IfNode : public Node {
private:
  Node* cond;
  Node* thenNode;
  Node* elseNode;

public:
  IfNode(Node* cond, Node* thenNode, Node* elseNode)
    : cond(cond), thenNode(thenNode), elseNode(elseNode) {}

  llvm::Value* codegen(Compiler*);
};


/**
 * A variable.
 * $value
 */

class NameNode : public Node {
private:
  std::string value;

public:
  NameNode(std::string value) : value(value) {}

  std::string getValue() { return value; }
  llvm::Value* codegen(Compiler*);
};


/**
 * Get a slot of an object.
 * $obj.$name
 */

class GetSlotNode : public Node {
private:
  Node* obj;
  std::string name;

public:
  GetSlotNode(Node* obj, std::string name)
    : obj(obj), name(name) {}

  llvm::Value* codegen(Compiler*);
};


/**
 * Set the slot of an object.
 * $obj.$name = $value
 */

class SetSlotNode : public Node {
private:
  Node* obj;
  std::string name;
  Node* value;

public:
  SetSlotNode(Node* obj, std::string name, Node* value)
    : obj(obj), name(name), value(value) {}

  llvm::Value* codegen(Compiler*);
};


/**
 * Call a slot of an object.
 * $obj.$name($args)
 */

class CallSlotNode : public Node {
private:
  Node* obj;
  std::string name;
  std::vector<Node*> args;

public:
  CallSlotNode(Node* obj, std::string name)
    : obj(obj), name(name) {}

  void addArg(Node* n) { args.push_back(n); }
  llvm::Value* codegen(Compiler*);
};

#endif
