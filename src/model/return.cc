#include "model/models.hh"

namespace pyrite {

  Value* ReturnModel::codegen(Compiler* c) {
    Function *F = c->builder->GetInsertBlock()->getParent();

    Value* retVal = value->codegen(c);

    if (F->getReturnType() != retVal->getType()) {
      throw "returned a wrong type in function " + F->getNameStr();
    }

    c->builder->CreateRet(retVal);

    return 0;
  }

}