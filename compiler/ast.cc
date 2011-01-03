#include "ast.hh"
#include "llvm.hh"

using namespace llvm;

Value* BinaryOpNode::codegen(Compiler* c) {
  return 0;
}

Value* NameNode::codegen(Compiler* c) {
  return 0;
}

Value* IntegerNode::codegen(Compiler* c) {
  return 0;
}

Value* StringNode::codegen(Compiler* c) {
  return 0;
}
