#include "model/models.hh"

namespace pyrite {

  std::string FunctionModel::get_name() {
    return name;
  }

  Value* FunctionModel::codegen(Compiler* c) {
    std::string n = c->env->format_function_name(name);

    // Push all argument types to the args vector
    std::vector<const Type*> argTypes;
    if (c->env->in_class()) {
      argTypes.push_back(c->ObjectTy);
    }
    for(unsigned int i = 0; i < args->size(); i++) {
      argTypes.push_back(c->ObjectTy);
    }

    // Set up the function
    FunctionType* FT = FunctionType::get(c->ObjectTy, argTypes, false);
    Function* F = Function::Create(FT, Function::ExternalLinkage, n, c->module);
    if (!block) return F;

    c->env->push();
    BasicBlock* MainBB = c->builder->GetInsertBlock(); 
    BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    c->builder->SetInsertPoint(BB);

    // Iterate all arguments
    Function::arg_iterator AI = F->arg_begin();
    if (c->env->in_class()) {
      AllocaInst* Alloca = c->builder->CreateAlloca(c->ObjectTy, 0, "self");
      c->builder->CreateStore(AI++, Alloca);
      c->env->symtab()->set("self", Alloca);
    }
    for (unsigned int i = 0; i < args->size(); i++, AI++) {
      AllocaInst* Alloca = c->builder->CreateAlloca(c->ObjectTy, 0, args->get(i).c_str());
      c->builder->CreateStore(AI, Alloca);
      c->env->symtab()->set(args->get(i), Alloca);
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
