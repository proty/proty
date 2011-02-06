#include "ast.hh"
#include "compiler.hh"

using namespace llvm;

Value* BlockNode::codegen(Compiler* c) {
  std::vector<Node*>::iterator it;
  Value* last = 0;
  for (it = nodes.begin(); it != nodes.end(); it++) {
    last = (*it)->codegen(c);
  }
  return last;
}

Value* BinaryOpNode::codegen(Compiler* c) {
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
  else if (op == "=") {
    std::string name = ((NameNode*)lhs)->getValue();
    Value* v = rhs->codegen(c);
    AllocaInst* alloca = (AllocaInst*)c->symtab->lookup(name);
    if (!alloca) {
      alloca = c->builder->CreateAlloca(v->getType(), 0, name);
      c->builder->CreateStore(v, alloca);
      c->symtab->store(name, alloca);
    }
    else {
      c->builder->CreateStore(v, alloca);
    }
    return alloca;
  }
  else return 0;

  Function* F = c->module->getFunction(instname);

  Value* l = lhs->codegen(c);
  Value* r = rhs->codegen(c);

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

Value* SlotNode::codegen(Compiler* c) {
  Function* getslot = c->module->getFunction("getslot");
  Value* object = obj->codegen(c);
  Value* slot = c->builder->CreateGlobalStringPtr(name.c_str(), ".str");
  return c->builder->CreateCall2(getslot, object, slot, name + "_tmp");
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
  NameNode* name = reinterpret_cast<NameNode*>(callee);
  if (name && name->getValue() == "load") {
    StringNode* loadName = reinterpret_cast<StringNode*>(args.at(0));
    std::string ln = loadName->getValue();
    c->loadModule(ln);

    Function* initFunc = c->module->getFunction(ln + "_init");
    return c->builder->CreateCall(initFunc, ln);
  }
  else {
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
}

Value* FunctionNode::codegen(Compiler* c) {
  int argc = args.size();

  std::vector<const Type*> argTypes;
  for(unsigned int i = 0; i < argc; i++) {
    argTypes.push_back(c->ObjectTy);
  }

  FunctionType* funcTy = FunctionType::get(c->ObjectTy, argTypes, false);
  Function* func = Function::Create(funcTy, Function::ExternalLinkage, "proty_func", c->module);

  // backup current block and setup new block
  BasicBlock* oldBB = c->builder->GetInsertBlock();
  BasicBlock* mainBB = BasicBlock::Create(getGlobalContext(), "entry", func);
  c->builder->SetInsertPoint(mainBB);

  // allocate all args
  Function::arg_iterator AI = func->arg_begin();
  for (unsigned int i = 0; i < argc; i++, AI++) {
      AI->setName(args.at(i));
      AllocaInst* alloca = c->builder->CreateAlloca(c->ObjectTy, 0, args.at(i));
      c->builder->CreateStore(AI, alloca);
      c->symtab->store(args.at(i), alloca);
  }

  c->symtab->enterScope();
  Value* returnValue = block->codegen(c);
  c->builder->CreateRet(returnValue);
  c->symtab->leaveScope();

  // restore old block
  c->builder->SetInsertPoint(oldBB);

  // get a pointer to the generated function
  std::vector<const Type*> funcPtrArgTypes;
  FunctionType* funcPtrTy = FunctionType::get(c->ObjectTy, funcPtrArgTypes, true);
  const Type* FunctionPtr = PointerType::get(funcPtrTy, 0);
  Value* funcPtr = c->builder->CreateBitCast(func, FunctionPtr);

  Function* newFunc = c->module->getFunction("newfunc");
  Value* argcValue= ConstantInt::get(Type::getInt32Ty(getGlobalContext()), argc);
  return c->builder->CreateCall2(newFunc, funcPtr, argcValue, "functmp");
}
