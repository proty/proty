#include "model/models.hh"

namespace pyrite {

  Value* BoolModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction("newbool");

    std::vector<Value*> args;
    args.push_back(ConstantInt::get(Type::getInt1Ty(getGlobalContext()), value));

    return c->builder->CreateCall(F, args.begin(), args.end(), "booltmp");
  }

}