#include "model/models.hh"

namespace pyrite {

  std::string FunctionModel::getName() {
    return name;
  }

  void FunctionModel::methodOf(ClassModel* klass) {
    this->klass = klass;
    name = klass->getName() + "::" + name; 
  }

  void FunctionModel::generatePrototype(Compiler* c) {
    // Push all argument types to the args vector
    std::vector<const Type*> argTypes;
    if (klass) argTypes.push_back(klass->getType(c));
    for(unsigned int i = 0; i < args->size(); i++) {
      argTypes.push_back(args->getType(i)->get(c));
    }

    FunctionType* FT = FunctionType::get(returnType->get(c), argTypes, args->getVarArg());
    Function::Create(FT, Function::ExternalLinkage, name, c->module);
  }

  void FunctionModel::generateFunction(Compiler* c) {
    if (!block) return;

    Function* F = c->module->getFunction(name);

    BasicBlock* MainBB = c->builder->GetInsertBlock(); 
    BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    c->builder->SetInsertPoint(BB);

    // Iterate all arguments
    Function::arg_iterator AI = F->arg_begin();
    if (klass) {
      AI->setName("self");
      AllocaInst* Alloca = c->builder->CreateAlloca(klass->getType(c), 0, "self");
      c->builder->CreateStore(AI, Alloca);
      c->symtab->store("self", Alloca);
      AI++;
    }
    for (unsigned int i = 0; i < args->size(); i++, AI++) {
      AI->setName(args->getName(i));
      AllocaInst* Alloca = c->builder->CreateAlloca(args->getType(i)->get(c), 0, args->getName(i).c_str());
      c->builder->CreateStore(AI, Alloca);
      c->symtab->store(args->getName(i), Alloca);
    }

    block->codegen(c);

    if (!c->builder->GetInsertBlock()->getTerminator()) {
      (new ReturnModel(new NilModel()))->codegen(c);
    }

    // Delete last block if empty
    if (c->builder->GetInsertBlock()->empty()) {
      c->builder->GetInsertBlock()->eraseFromParent();
    }

    c->builder->SetInsertPoint(MainBB);

    // Check and optimize the function
    verifyFunction(*F);
    c->fpm->run(*F);
  }

}
