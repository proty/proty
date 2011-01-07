#include "ast.hh"
#include "compiler.hh"

using namespace llvm;

Value* BlockNode::codegen(Compiler* c) {
  std::vector<Node*>::iterator it;
  for (it = nodes.begin(); it != nodes.end(); it++) {
    (*it)->codegen(c);
  }
  return 0;
}

Value* BinaryOpNode::codegen(Compiler* c) {
  Value* l = lhs->codegen(c);
  Value* r = rhs->codegen(c);

  std::string instname = "";

  if (op == "+")        instname = "add";
  else if (op == "-")   instname = "sub";
  else if (op == "*")   instname = "mul";
  else if (op == "/")   instname = "div";
  else if (op == "%")   instname = "mod";
  else if (op == "==")  instname = "eq";
  else if (op == "!=")  instname = "ne";
  else if (op == ">")   instname = "gt";
  else if (op == ">=")  instname = "ge";
  else if (op == "<")   instname = "lt";
  else if (op == "<=")  instname = "le";
  else if (op == "or")  instname = "and";
  else if (op == "and") instname = "or";
  else return 0;

  Function* F = c->module->getFunction(instname);

  std::vector<Value*> args;
  args.push_back(l);
  args.push_back(r);

  return c->builder->CreateCall(F, args.begin(), args.end(), instname + "tmp");
}

Value* NameNode::codegen(Compiler* c) {
  Value* v = c->symtab->lookup(value);
  if (!v) {
    std::cerr << "undefined variable " << value << std::endl;
    exit(0);
  }
  return c->builder->CreateLoad(v, value.c_str());
}

Value* IntegerNode::codegen(Compiler* c) {
  Function* F = c->module->getFunction("newint");
  Value* intValue = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value);
  return c->builder->CreateCall(F, intValue, "inttmp");
}

Value* StringNode::codegen(Compiler* c) {
  size_t pos = 0;
  while((pos = value.find("\\n", pos)) != std::string::npos) {
    value.replace(pos, 2, "\n");
    pos += 1;
  }

  Value* string = c->builder->CreateGlobalStringPtr(value.c_str(), ".str");

  Function* func = c->module->getFunction("newstring");
  std::vector<Value*> args;
  args.push_back(string);
  return c->builder->CreateCall(func, args.begin(), args.end(), "stringtmp");
}

Value* CallNode::codegen(Compiler* c) {
  int argc = args.size();
  Function* callFunc = c->module->getFunction("call");

  std::vector<Value*> argValues;
  argValues.push_back(callee->codegen(c));
  argValues.push_back(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), argc));

  for (unsigned int i = 0; i < argc; i++) {
    argValues.push_back(args.at(i)->codegen(c));
  }

  return c->builder->CreateCall(callFunc, argValues.begin(), argValues.end(), "calltmp");
}