#include "model/models.hh"

namespace pyrite {
  
  Value* IntegerModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction("Integer::new");

    std::vector<Value*> argValues;
    argValues.push_back(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value));

    return c->builder->CreateCall(F, argValues.begin(), argValues.end(), "inttmp");
  }

}
