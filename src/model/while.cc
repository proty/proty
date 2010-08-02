#include "model/models.hh"

namespace pyrite {

  Value* WhileModel::codegen(Compiler* c) {
    Function* Function = c->builder->GetInsertBlock()->getParent();
    
    BasicBlock* LoopBB = BasicBlock::Create(getGlobalContext(), "loop", Function);
    BasicBlock* AfterBB = BasicBlock::Create(getGlobalContext(), "afterloop", Function);
    c->builder->CreateBr(LoopBB);
    c->builder->SetInsertPoint(LoopBB);
    
    block->codegen(c);
    
    Value* condition = cond->codegen(c);
    
    condition = c->builder->CreateIsNotNull(condition);
    Value* EndCond = c->builder->CreateICmpEQ(condition, ConstantInt::get(Type::getInt1Ty(getGlobalContext()), 1), "loopcond");
    c->builder->CreateCondBr(EndCond, LoopBB, AfterBB);
    
    c->builder->SetInsertPoint(AfterBB);
    
    return 0;
  }

}
