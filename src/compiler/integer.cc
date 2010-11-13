#include "compiler/models.hh"

namespace pyrite {

  Value* IntegerModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction("newint");
    Value* intValue = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value);
    return c->builder->CreateCall(F, intValue, "inttmp");
  }

}
