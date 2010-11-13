#include "model/models.hh"

namespace pyrite {

  Value* BoolModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction("newbool");
    Value* boolValue = ConstantInt::get(Type::getInt1Ty(getGlobalContext()), value);
    return c->builder->CreateCall(F, boolValue, "booltmp");
  }

}