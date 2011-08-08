#include "ast.hh"
#include "compiler.hh"
#include "symtab.hh"
#include "error.hh"
#include "llvm.hh"

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
  GetSlotNode* getslot = new GetSlotNode(lhs, op);

  CallNode* call = new CallNode(getslot);
  call->setSelf(lhs);
  call->addArg(rhs);

  return call->codegen(c);
}

Value* SubscriptNode::codegen(Compiler* c) {
  std::string op = value ? "[]=" : "[]";

  GetSlotNode* getslot = new GetSlotNode(object, op);

  CallNode* call = new CallNode(getslot);
  call->setSelf(object);
  call->addArg(key);
  if (value) call->addArg(value);

  return call->codegen(c);
}

Value* NameNode::codegen(Compiler* c) {
  Value* v = c->symtab->lookup(value);
  if (!v) {
    throw new CompileError("undefined variable '" + value + "'");
  }
  return c->builder->CreateLoad(v, value.c_str());
}

Value* AssignNode::codegen(Compiler* c) {
  Value* v = value->codegen(c);
  Value* variable = c->symtab->lookup(name);
  if (!variable) {
    if (c->interactive && c->toplevel) {
      variable = new GlobalVariable(*c->module, c->getObjectTy(), false,
                                  GlobalValue::ExternalLinkage,
                                  UndefValue::get(c->getObjectTy()), name);
      c->builder->CreateStore(v, variable);
      c->symtab->store(name, variable);
    }
    else {
      variable = c->builder->CreateAlloca(c->getObjectTy(), 0, name);
      c->builder->CreateStore(v, variable);
      c->symtab->store(name, variable);
    }
  }
  else {
    c->builder->CreateStore(v, variable);
  }
  return variable;
}

Value* GetSlotNode::codegen(Compiler* c) {
  Function* getslot = c->module->getFunction("Object_getSlot");
  Value* object = obj->codegen(c);
  Value* slot = c->builder->CreateGlobalStringPtr(name.c_str(), ".str");
  return c->builder->CreateCall2(getslot, object, slot, name + "_tmp");
}

Value* SetSlotNode::codegen(Compiler* c) {
  Function* setslot = c->module->getFunction("Object_setSlot");
  Value* object = obj->codegen(c);
  Value* slot = c->builder->CreateGlobalStringPtr(name.c_str(), ".str");
  Value* v = value->codegen(c);
  return c->builder->CreateCall3(setslot, object, slot, v, "set_" + name + "_tmp");
}

Value* CallSlotNode::codegen(Compiler* c) {
  ValueNode* objv = new ValueNode(obj->codegen(c));
  GetSlotNode* slot = new GetSlotNode(objv, name);
  ValueNode* slotv = new ValueNode(slot->codegen(c));

  CallNode* call = new CallNode(slotv);
  call->setSelf(objv);

  for (int i = 0; i < args.size(); i++) {
    call->addArg(args.at(i));
  }

  return call->codegen(c);
}

Value* ModuleMemberNode::codegen(Compiler* c) {
  Module* mod;

  // check if the module is already loaded, if not, load it!
  std::map<std::string, Module*>::iterator it = c->modules.find(module);
  if (it == c->modules.end()) {
    mod = c->loadModule(module);
  }
  else {
    mod = (*it).second;
  }

  std::string obj_name = "prmod_" + module + "_" + name;

  Value* obj = c->module->getNamedValue(obj_name);

  // if this object is not declare in the current module,
  // verfiy that it exists and declare it.
  if (!obj && (obj = mod->getNamedValue(obj_name))) {
    obj = c->declareExternObject(obj_name);
  }

  if (!obj) {
    throw new CompileError("member '" + name + "' of module '"
                           + module + "' not found.");
  }

  return c->builder->CreateLoad(obj);
}

Value* IntegerNode::codegen(Compiler* c) {
  Function* F = c->module->getFunction("Integer_new");
  Value* intValue = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value);
  return c->builder->CreateCall(F, intValue, "inttmp");
}

Value* FloatNode::codegen(Compiler* c) {
  Function* F = c->module->getFunction("Float_new");
  Value* doubleValue = ConstantFP::get(getGlobalContext(), APFloat(value));
  return c->builder->CreateCall(F, doubleValue, "floattmp");
}

Value* StringNode::codegen(Compiler* c) {
  Value* string = c->builder->CreateGlobalStringPtr(value.c_str(), ".str");

  Function* func = c->module->getFunction("String_new");
  return c->builder->CreateCall(func, string, "stringtmp");
}

Value* ListNode::codegen(Compiler* c) {
  Function* F = c->module->getFunction("List_new");
  Value* list = c->builder->CreateCall(F, "listtmp");

  Function* append = c->module->getFunction("List_append");
  for (int i = 0; i < objects.size(); i++) {
    Value* v = objects.at(i)->codegen(c);
    c->builder->CreateCall2(append, list, v);
  }

  return list;
}

Value* HashNode::codegen(Compiler* c) {
  Function* F = c->module->getFunction("Hash_new");
  Value* hash = c->builder->CreateCall(F, "hashtmp");

  Function* set = c->module->getFunction("Hash_set");

  std::map<Node*, Node*>::iterator it;
  for(it = items.begin(); it != items.end(); it++) {
    Value* k = it->first->codegen(c);
    Value* v = it->second->codegen(c);
    c->builder->CreateCall3(set, hash, k, v);
  }

  return hash;
}

Value* SymbolNode::codegen(Compiler* c) {
  Value* string = c->builder->CreateGlobalStringPtr(name.c_str(), ".sym");

  Function* func = c->module->getFunction("Symbol_get");
  return c->builder->CreateCall(func, string, "symboltmp");
}

Value* CallNode::codegen(Compiler* c) {
  int argc = args.size();
  Function* callFunc = c->module->getFunction("Object_call");

  std::vector<Value*> argValues;
  argValues.push_back(callee->codegen(c));
  argValues.push_back(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), argc+1));

  if (self) {
    argValues.push_back(self->codegen(c));
  }
  else {
    argValues.push_back(c->getNil());
  }

  for (int i = 0; i < argc; i++) {
    argValues.push_back(args.at(i)->codegen(c));
  }

  // are we in a try block?
  if (!c->unwind.empty()) {
    Function* func = c->builder->GetInsertBlock()->getParent();
    BasicBlock* invcontBB = BasicBlock::Create(getGlobalContext(), "invcont",
                                               func, c->unwind.top());

    Value* call = c->builder->CreateInvoke(callFunc, invcontBB, c->unwind.top(),
                                           argValues.begin(), argValues.end(), "invtmp");

    c->builder->SetInsertPoint(invcontBB);

    return call;
  }
  else {
    return c->builder->CreateCall(callFunc, argValues.begin(), argValues.end(), "calltmp");
  }
}

Value* FunctionNode::codegen(Compiler* c) {
  int argc = args.size();

  std::vector<const Type*> argTypes;
  for(int i = 0; i < argc; i++) {
    argTypes.push_back(c->getObjectTy());
  }

  FunctionType* funcTy = FunctionType::get(c->getObjectTy(), argTypes, false);
  Function* func = Function::Create(funcTy, Function::InternalLinkage, "proty_func", c->module);

  // backup current block and setup new block
  BasicBlock* oldBB = c->builder->GetInsertBlock();
  BasicBlock* mainBB = BasicBlock::Create(getGlobalContext(), "entry", func);
  c->builder->SetInsertPoint(mainBB);

  // allocate all args
  Function::arg_iterator AI = func->arg_begin();
  for (int i = 0; i < argc; i++, AI++) {
      AI->setName(args.at(i));
      AllocaInst* alloca = c->builder->CreateAlloca(c->getObjectTy(), 0, args.at(i));
      c->builder->CreateStore(AI, alloca);
      c->symtab->store(args.at(i), alloca);
  }

  bool was_toplevel = c->toplevel;
  c->toplevel = false;
  c->symtab->enterScope();
  Value* returnValue = block->codegen(c);
  BasicBlock* currBB = c->builder->GetInsertBlock();
  if (!currBB->getTerminator()) {
    if (returnValue) {
      c->builder->CreateRet(returnValue);
    }
    else {
      Value* Qnil = c->builder->CreateLoad(c->getNil());
      c->builder->CreateRet(Qnil);
    }
  }
  c->symtab->leaveScope();
  c->toplevel = was_toplevel;

  // restore old block
  c->builder->SetInsertPoint(oldBB);

  verifyFunction(*func);
  c->fpm->run(*func);

  // get a pointer to the generated function
  std::vector<const Type*> funcPtrArgTypes;
  funcPtrArgTypes.push_back(c->getObjectTy());
  FunctionType* funcPtrTy = FunctionType::get(c->getObjectTy(), funcPtrArgTypes, true);
  Type* FunctionPtr = PointerType::get(funcPtrTy, 0);
  Value* funcPtr = c->builder->CreateBitCast(func, FunctionPtr);

  Function* newFunc = c->module->getFunction("Function_new");
  Value* argcValue= ConstantInt::get(Type::getInt32Ty(getGlobalContext()), argc);
  return c->builder->CreateCall2(newFunc, funcPtr, argcValue, "functmp");
}

Value* IfNode::codegen(Compiler* c) {
  Function* func = c->builder->GetInsertBlock()->getParent();
  BasicBlock* ThenBB = BasicBlock::Create(getGlobalContext(), "then", func);
  BasicBlock* ElseBB = BasicBlock::Create(getGlobalContext(), "else");
  BasicBlock* MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");

  CallSlotNode* boolean = new CallSlotNode(cond, "bool");

  Value* Qtrue = c->builder->CreateLoad(c->getBool(true));
  Value* EndCond = c->builder->CreateICmpEQ(boolean->codegen(c), Qtrue);
  c->builder->CreateCondBr(EndCond, ThenBB, ElseBB);

  c->builder->SetInsertPoint(ThenBB);
  thenNode->codegen(c);
  if (!ThenBB->getTerminator()) c->builder->CreateBr(MergeBB);

  func->getBasicBlockList().push_back(ElseBB);
  c->builder->SetInsertPoint(ElseBB);
  if (elseNode) {
    elseNode->codegen(c);
  }
  if (!ElseBB->getTerminator()) c->builder->CreateBr(MergeBB);

  func->getBasicBlockList().push_back(MergeBB);
  c->builder->SetInsertPoint(MergeBB);

  return 0;
}

Value* WhileNode::codegen(Compiler* c) {
  Function* func = c->builder->GetInsertBlock()->getParent();
  BasicBlock* CondBB = BasicBlock::Create(getGlobalContext(), "cond", func);
  BasicBlock* LoopBB = BasicBlock::Create(getGlobalContext(), "loop", func);
  BasicBlock* AfterBB = BasicBlock::Create(getGlobalContext(), "afterloop", func);

  c->builder->CreateBr(CondBB);
  c->builder->SetInsertPoint(CondBB);

  CallSlotNode* boolean = new CallSlotNode(cond, "bool");

  Value* Qtrue = c->builder->CreateLoad(c->getBool(true));
  Value* EndCond = c->builder->CreateICmpEQ(boolean->codegen(c), Qtrue);
  c->builder->CreateCondBr(EndCond, LoopBB, AfterBB);

  c->builder->SetInsertPoint(LoopBB);
  block->codegen(c);
  c->builder->CreateBr(CondBB);

  c->builder->SetInsertPoint(AfterBB);
  return 0;
}

Value* TryNode::codegen(Compiler* c) {
  Function* func = c->builder->GetInsertBlock()->getParent();
  BasicBlock* tryBB = BasicBlock::Create(getGlobalContext(), "try", func);
  BasicBlock* catchBB = BasicBlock::Create(getGlobalContext(), "catch", func);
  BasicBlock* elseBB = BasicBlock::Create(getGlobalContext(), "else", func);
  BasicBlock* finallyBB = BasicBlock::Create(getGlobalContext(), "finally", func);

  c->builder->CreateBr(tryBB);

  // try
  c->builder->SetInsertPoint(tryBB);
  c->symtab->enterScope();

  c->unwind.push(catchBB);
  tryBlock->codegen(c);
  c->unwind.pop();

  c->symtab->leaveScope();
  c->builder->CreateBr(elseBB);

  // catch
  c->builder->SetInsertPoint(catchBB);
  Function* eh_exception = c->module->getFunction("llvm.eh.exception");
  Value* exception_ptr = c->builder->CreateCall(eh_exception);

  Function* proty_personality = c->module->getFunction("proty_personality");
  Value* personality = c->builder->CreatePointerCast(proty_personality, Type::getInt8PtrTy(getGlobalContext()));
  Function* eh_selector = c->module->getFunction("llvm.eh.selector");
  Value* selector = c->builder->CreateCall3(eh_selector, exception_ptr, personality,
                                            c->builder->CreateLoad(c->getNil()));

  c->symtab->enterScope();

  //AllocaInst* alloca = c->builder->CreateAlloca(c->getObjectTy(), 0, ename);
  //c->builder->CreateStore(exception, alloca);
  //c->symtab->store(ename, alloca);

  catchBlock->codegen(c);
  c->symtab->leaveScope();

  c->builder->CreateBr(finallyBB);

  // else
  c->builder->SetInsertPoint(elseBB);

  if (elseBlock) {
    c->symtab->enterScope();
    elseBlock->codegen(c);
    c->symtab->leaveScope();
  }

  c->builder->CreateBr(finallyBB);

  // finally
  c->builder->SetInsertPoint(finallyBB);

  return 0;
}

Value* PropertyNode::codegen(Compiler* c) {
  if (name == "export") {
    for (int i = 0; i < values.size(); i++) {
      std::string varname = "prmod_";
      varname.append(c->module->getModuleIdentifier());
      varname.append("_" + values.at(i));

      Value* variable = new GlobalVariable(*c->module, c->getObjectTy(), false,
                                           GlobalValue::ExternalLinkage,
                                           UndefValue::get(c->getObjectTy()), varname);

      c->symtab->store(values.at(i), variable);
    }
  }
  else {
    throw new CompileError("unknown property name '" + name + "'");
  }

  return 0;
}
