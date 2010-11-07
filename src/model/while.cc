#include "model/models.hh"

namespace pyrite {

  Value* WhileModel::codegen(Compiler* c) {
    Function* func = c->builder->GetInsertBlock()->getParent();
    BasicBlock* CondBB = BasicBlock::Create(getGlobalContext(), "cond", func);
    BasicBlock* LoopBB = BasicBlock::Create(getGlobalContext(), "loop", func);
    BasicBlock* AfterBB = BasicBlock::Create(getGlobalContext(), "afterloop", func);

    // CondBB
    c->builder->CreateBr(CondBB);
    c->builder->SetInsertPoint(CondBB);

    Function* CondF = c->module->getFunction("tobool");
    std::vector<Value*> args;
    args.push_back(cond->codegen(c));
    Value* EndCond = c->builder->CreateCall(CondF, args.begin(), args.end(), "booltmp");

    c->builder->CreateCondBr(EndCond, LoopBB, AfterBB);

    // LoopBB
    c->builder->SetInsertPoint(LoopBB);
    block->codegen(c);
    c->builder->CreateBr(CondBB);

    // AfterBB
    c->builder->SetInsertPoint(AfterBB);

    return 0;
  }

}