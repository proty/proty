#ifndef PROTY_AST_HH
#define PROTY_AST_HH

#include <iostream>
#include <vector>
#include <map>

namespace llvm { class Value; }
class Compiler;

class Node {
public:
  virtual ~Node() {}
  virtual llvm::Value* codegen(Compiler*) = 0;
  virtual std::string getValue() { return ""; }
};


/**
 * A code block.
 * $nodes end | { $nodes }
 */

class BlockNode : public Node {
private:
  std::vector<Node*> nodes;

public:
  ~BlockNode() {
    for (int i = 0; i < nodes.size(); i++)
      delete nodes[i];
  }

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
 * A float object.
 */

class FloatNode : public Node {
private:
  double value;

public:
  FloatNode(double value) : value(value) {}

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
 * A list object.
 * [$objects]
 */

class ListNode : public Node {
private:
  std::vector<Node*> objects;

public:
  ListNode() {}

  void add(Node* n) { objects.push_back(n); }
  llvm::Value* codegen(Compiler*);
};


/**
 * A hash object.
 * {($key: $value)*}
 */

class HashNode : public Node {
private:
  std::map<Node*, Node*> items;

public:
  HashNode() {}

  void add(Node* key, Node* value) { items[key] = value; }
  llvm::Value* codegen(Compiler*);
};


/**
 * A symbol.
 * :$name
 */

class SymbolNode : public Node {
private:
  std::string name;

public:
  SymbolNode(std::string name) : name(name) {}

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
  ~BinaryOpNode() { delete lhs; delete rhs; }

  llvm::Value* codegen(Compiler*);
};


/**
 * A subscript operation.
 * $object[$value]
 */

class SubscriptNode : public Node {
private:
  Node* object;
  Node* value;

public:
  SubscriptNode(Node* object, Node* value)
    : object(object), value(value) {};

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
  ~CallNode() { delete callee; }

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
  ~FunctionNode() { delete block; }

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
  ~IfNode() { delete cond; delete thenNode; delete elseNode; }

  llvm::Value* codegen(Compiler*);
};


/**
 * A while statement.
 * while $cond
 *   $block
 * end
 */

class WhileNode : public Node {
private:
  Node* cond;
  Node* block;

public:
  WhileNode(Node* cond, Node* block)
    : cond(cond), block(block) {}
  ~WhileNode() { delete cond; delete block; }

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
 * A variable assignment.
 * $name = $value
 */

class AssignNode : public Node {
private:
  std::string name;
  Node* value;

public:
  AssignNode(std::string name, Node* value)
    : name(name), value(value) {}
  ~AssignNode() { delete value; }

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
  ~GetSlotNode() { delete obj; }

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
  ~SetSlotNode() { delete obj; delete value; }

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
  ~CallSlotNode() { delete obj; }

  void addArg(Node* n) { args.push_back(n); }
  llvm::Value* codegen(Compiler*);
};


/**
 * A try statement.
 * try
 *   $tryBlock
 * catch $eName
 *   $catchBlock
 * else
 *   $elseBlock
 * end
 */

class TryNode : public Node {
private:
  Node* tryBlock;
  std::string ename;
  Node* catchBlock;
  Node* elseBlock;

public:
  TryNode(Node* tryBlock, std::string ename, Node* catchBlock, Node* elseBlock)
    : tryBlock(tryBlock), ename(ename), catchBlock(catchBlock), elseBlock(elseBlock) {}

  llvm::Value* codegen(Compiler*);
};


/**
 * Holds a llvm::Value.
 */

class ValueNode : public Node {
private:
  llvm::Value* value;

public:
  ValueNode(llvm::Value* value) : value(value) {}

  llvm::Value* codegen(Compiler*) { return value; }
};

#endif
