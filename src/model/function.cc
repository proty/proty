#include "model/models.hh"

namespace pyrite {

  std::string FunctionModel::get_name() {
    return name;
  }

  void FunctionModel::generate_prototype(Compiler* c) {
    // Push all argument types to the args vector
    std::vector<const Type*> argTypes;
    for(unsigned int i = 0; i < args->size(); i++) {
      argTypes.push_back(args->getType(i)->get(c));
    }

    FunctionType* FT = FunctionType::get(returnType->get(c), argTypes, false);
    Function::Create(FT, Function::ExternalLinkage, name, c->module);
  }

  void FunctionModel::generate_function(Compiler* c) {
    Function* F = c->module->getFunction(name);

    BasicBlock* MainBB = c->builder->GetInsertBlock(); 
    BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    c->builder->SetInsertPoint(BB);

    // Iterate all arguments
    Function::arg_iterator AI = F->arg_begin();
    for (unsigned int i = 0; i < args->size(); i++, AI++) {
      AllocaInst* Alloca = c->builder->CreateAlloca(args->getType(i)->get(c), 0, args->getName(i).c_str());
      c->builder->CreateStore(AI, Alloca);
    }

    block->codegen(c);

    if (!BB->getTerminator()) {
      // @todo return nil
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
