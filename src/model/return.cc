#include "model/models.hh"

namespace pyrite {

  Value* ReturnModel::codegen(Compiler* c) {
    Value* retVal = value->codegen(c);
    c->builder->CreateRet(retVal);

    return 0;
  }

}