#include "model/models.hh"
#include <iostream>

namespace pyrite {

  Value* AssignModel::codegen(Compiler* c) {
    Value* v = value->codegen(c);
    const Type* t = v->getType();

    AllocaInst* Alloca = 0;
    if (!Alloca) {
      Function* Function = c->builder->GetInsertBlock()->getParent();
      IRBuilder<> TmpB(&Function->getEntryBlock(), Function->getEntryBlock().begin());
      Alloca = TmpB.CreateAlloca(t, 0, name);
    }
    c->builder->CreateStore(v, Alloca);
    c->symtab->store(name, Alloca);

    return Alloca;
  }

}
