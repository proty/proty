#include "model/models.hh"

namespace pyrite {

  Value* IfModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction("tobool");

    Function* func = c->builder->GetInsertBlock()->getParent();
    BasicBlock* ThenBB = BasicBlock::Create(getGlobalContext(), "then", func);
    BasicBlock* ElseBB = BasicBlock::Create(getGlobalContext(), "else");
    BasicBlock* MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");

    std::vector<Value*> args;
    args.push_back(cond->codegen(c));
    Value* EndCond = c->builder->CreateCall(F, args.begin(), args.end(), "booltmp");
    c->builder->CreateCondBr(EndCond, ThenBB, ElseBB);

    c->builder->SetInsertPoint(ThenBB);
    thenBlock->codegen(c);
    if (!ThenBB->getTerminator()) c->builder->CreateBr(MergeBB);

    func->getBasicBlockList().push_back(ElseBB);
    c->builder->SetInsertPoint(ElseBB);
    if (elseBlock) {
        elseBlock->codegen(c);
    }
    if (!ElseBB->getTerminator()) c->builder->CreateBr(MergeBB);

    func->getBasicBlockList().push_back(MergeBB);
    c->builder->SetInsertPoint(MergeBB);

    return 0;
  }

}
