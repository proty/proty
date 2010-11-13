#include "compiler/models.hh"

namespace pyrite {

  Value* AttributeModel::codegen(Compiler* c) {
    Value* inst = instance->codegen(c);
    Value* keyValue = (new StringModel(name))->codegen(c);

    if (!value) {
      Function* getattrFunc = c->module->getFunction("getattr");
      return c->builder->CreateCall2(getattrFunc, inst, keyValue, "attrtmp");
    }
    else {
      Value* valueValue = value->codegen(c);
      Function* setattrFunc = c->module->getFunction("setattr");
      return c->builder->CreateCall3(setattrFunc, inst, keyValue, valueValue, "attrtmp");
    }
  }

}
