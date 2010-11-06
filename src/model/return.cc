#include "model/models.hh"

namespace pyrite {

  Value* ReturnModel::codegen(Compiler* c) {
    Function *F = c->builder->GetInsertBlock()->getParent();

    Value* retVal = value->codegen(c);

    c->builder->CreateRet(retVal);

    return 0;
  }

}