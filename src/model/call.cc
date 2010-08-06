#include "model/models.hh"

namespace pyrite {

  Value* CallModel::codegen(Compiler* c) {
    Function* F = c->module->getFunction(name);
    if (!F) throw "Function '" + name + "' is not defined.";

    std::vector<Value*> argValues;
    for (unsigned int i = 0; i < args->size(); i++) {
      argValues.push_back(args->get(i)->codegen(c));
    }

    if (!F->getReturnType()->isVoidTy())
      return c->builder->CreateCall(F, argValues.begin(), argValues.end(), "calltmp");
    else
      return c->builder->CreateCall(F, argValues.begin(), argValues.end());
  }

}
