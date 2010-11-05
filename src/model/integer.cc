#include "model/models.hh"

namespace pyrite {

  Value* IntegerModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction("newint");

    std::vector<Value*> args;
    args.push_back(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value));

    return c->builder->CreateCall(F, args.begin(), args.end(), "inttmp");
  }

}
