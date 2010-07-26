#include "model/models.hh"

namespace pyrite {

  Value* IfModel::codegen(Compiler* c) {
    Value* condition = cond->codegen(c);
    condition = c->builder->CreateICmpEQ(condition, ConstantInt::get(Type::getInt1Ty(getGlobalContext()), 1), "ifcond");
    
    Function* Function = c->builder->GetInsertBlock()->getParent();
    BasicBlock* ThenBB = BasicBlock::Create(getGlobalContext(), "then", Function);
    BasicBlock* ElseBB = BasicBlock::Create(getGlobalContext(), "else");
    BasicBlock* MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");

    c->builder->CreateCondBr(condition, ThenBB, ElseBB);
    c->builder->SetInsertPoint(ThenBB);
    for (unsigned int i = 0; i < thenBlock->size(); i++) {
      thenBlock->get(i)->codegen(c);
    }
    if (!ThenBB->getTerminator()) c->builder->CreateBr(MergeBB);
    
    Function->getBasicBlockList().push_back(ElseBB);
    c->builder->SetInsertPoint(ElseBB);
    if (elseBlock) {
      for (unsigned int i = 0; i < elseBlock->size(); i++) {
        elseBlock->get(i)->codegen(c);
      }
    }
    if (!ElseBB->getTerminator()) c->builder->CreateBr(MergeBB);

    Function->getBasicBlockList().push_back(MergeBB);
    c->builder->SetInsertPoint(MergeBB);

    return 0;
  }

}
