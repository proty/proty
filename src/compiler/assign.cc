#include "compiler/models.hh"

namespace pyrite {

  Value* AssignModel::codegen(Compiler* c) {
    Value* v = value->codegen(c);
    const Type* t = v->getType();

    AllocaInst* alloca = (AllocaInst*)c->symtab->lookup(name);
    if (!alloca) {
      alloca = c->builder->CreateAlloca(t, 0, name);
      c->builder->CreateStore(v, alloca);
      c->symtab->store(name, alloca);
    }
    else {
      c->builder->CreateStore(v, alloca);
    }

    return alloca;
  }

}
