#include "model/models.hh"

namespace pyrite {

  std::string FunctionModel::get_name() {
    return name;
  }

  Value* FunctionModel::codegen(Compiler* c) {
    std::string n = c->env->format_function_name(name);

    // Push all argument types to the args vector
    std::vector<const Type*> argTypes;
    for(unsigned int i = 0; i < args->size(); i++) {
      argTypes.push_back(args->getType(i)->get(c));
    }

    // Set up the function
    FunctionType* FT = FunctionType::get(returnType->get(c), argTypes, false);
    Function* F = Function::Create(FT, Function::ExternalLinkage, n, c->module);
    if (!block) return F;

    c->env->push();
    BasicBlock* MainBB = c->builder->GetInsertBlock(); 
    BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    c->builder->SetInsertPoint(BB);

    // Iterate all arguments
    Function::arg_iterator AI = F->arg_begin();
    for (unsigned int i = 0; i < args->size(); i++, AI++) {
      AllocaInst* Alloca = c->builder->CreateAlloca(args->getType(i)->get(c), 0, args->getName(i).c_str());
      c->builder->CreateStore(AI, Alloca);
      c->env->symtab()->set(args->getName(i), Alloca);
    }

    for (unsigned int i = 0; i < block->size(); i++) {
      block->get(i)->codegen(c);
    }

    if (!BB->getTerminator()) {
      // @todo return nil
    }

    // Delete last block if empty
    if (c->builder->GetInsertBlock()->empty()) {
      c->builder->GetInsertBlock()->eraseFromParent();
    }

    c->builder->SetInsertPoint(MainBB);
    c->env->pop();

    // Check and optimize the function
    verifyFunction(*F);
    c->fpm->run(*F);

    return F;
  }

}
