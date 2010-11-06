#include "model/models.hh"

namespace pyrite {

  Value* IfModel::codegen(Compiler* c) {
    MethodCallModel* call = new MethodCallModel(cond, "bool");
    Value* condition = call->codegen(c);
    call = new MethodCallModel(new ValueModel(condition), "get_value");
    condition = call->codegen(c);

    condition = c->builder->CreateICmpEQ(condition, ConstantInt::get(Type::getInt1Ty(getGlobalContext()), 1), "ifcond");

    Function* Function = c->builder->GetInsertBlock()->getParent();
    BasicBlock* ThenBB = BasicBlock::Create(getGlobalContext(), "then", Function);
    BasicBlock* ElseBB = BasicBlock::Create(getGlobalContext(), "else");
    BasicBlock* MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");

    c->builder->CreateCondBr(condition, ThenBB, ElseBB);
    c->builder->SetInsertPoint(ThenBB);
    thenBlock->codegen(c);
    if (!ThenBB->getTerminator()) c->builder->CreateBr(MergeBB);

    Function->getBasicBlockList().push_back(ElseBB);
    c->builder->SetInsertPoint(ElseBB);
    if (elseBlock) {
        elseBlock->codegen(c);
    }
    if (!ElseBB->getTerminator()) c->builder->CreateBr(MergeBB);

    Function->getBasicBlockList().push_back(MergeBB);
    c->builder->SetInsertPoint(MergeBB);

    return 0;
  }

}
