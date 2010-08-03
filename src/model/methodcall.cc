#include "model/models.hh"

namespace pyrite {

  Value* MethodCallModel::codegen(Compiler* c) {
    Value* inst = instance->codegen(c);
    const Type* instTy = inst->getType();
    while (instTy->isPointerTy()) {
      instTy = instTy->getContainedType(0);
    }

    std::string classname = c->module->getTypeName(instTy);
    
    Function* F;
    std::vector<Value*> argValues;

    F = c->module->getFunction(classname + "." + name);
    for (unsigned int i = 0; i < args->size(); i++) {
      argValues.push_back(args->get(i)->codegen(c));
    }

    return c->builder->CreateCall(F, argValues.begin(), argValues.end(), "calltmp");
  }

}