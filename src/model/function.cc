#include "model/models.hh"

namespace pyrite {

  Value* FunctionModel::codegen(Compiler* c) {
    int argc = args->size();

    std::vector<const Type*> argTypes;
    for(unsigned int i = 0; i < argc; i++) {
      argTypes.push_back(c->ObjectTy);
    }

    FunctionType* funcTy = FunctionType::get(c->ObjectTy, argTypes, false);
    Function* func = Function::Create(funcTy, Function::ExternalLinkage, name, c->module);

    BasicBlock* oldBB = c->builder->GetInsertBlock();
    BasicBlock* mainBB = BasicBlock::Create(getGlobalContext(), "entry", func);
    c->builder->SetInsertPoint(mainBB);

    Function::arg_iterator AI = func->arg_begin();
    for (unsigned int i = 0; i < argc; i++, AI++) {
      AI->setName(args->getName(i));
      AllocaInst* alloca = c->builder->CreateAlloca(c->ObjectTy, 0, args->getName(i).c_str());
      c->builder->CreateStore(AI, alloca);
      c->symtab->store(args->getName(i), alloca);
    }

    block->codegen(c);

    c->builder->SetInsertPoint(oldBB);

    // Check and optimize the function
    verifyFunction(*func);
    c->fpm->run(*func);

    std::vector<const Type*> funcPtrArgTypes;
    FunctionType* funcPtrTy = FunctionType::get(c->ObjectTy, funcPtrArgTypes, true);
    const Type* FunctionPtr = PointerType::get(funcPtrTy, 0);
    Value* funcPtr = c->builder->CreateBitCast(func, FunctionPtr);

    Function* newFuncInst = c->module->getFunction("newfunc");
    std::vector<Value*> newFuncArgs;
    Value* argcValue= ConstantInt::get(Type::getInt32Ty(getGlobalContext()), argc);
    ValueModel* funcObj = new ValueModel(c->builder->CreateCall2(newFuncInst, funcPtr, argcValue, "functmp"));

    AssignModel* assignment = new AssignModel(name, funcObj);
    assignment->codegen(c);

    return 0;
  }

}
