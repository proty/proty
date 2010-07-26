#include "model/models.hh"

namespace pyrite {

  Value* AssignModel::codegen(Compiler* c) {
    Value* v = value->codegen(c);
    const Type* t = v->getType();

    AllocaInst* Alloca = c->env->symtab()->get(name);
    if (!Alloca) {
      Function* Function = c->builder->GetInsertBlock()->getParent();
      IRBuilder<> TmpB(&Function->getEntryBlock(), Function->getEntryBlock().begin());
      Alloca = TmpB.CreateAlloca(t, 0, name);
    }

    c->builder->CreateStore(v, Alloca);
    c->env->symtab()->set(name, Alloca);

    return 0;
  }

}
