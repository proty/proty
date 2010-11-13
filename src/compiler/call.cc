#include "compiler/models.hh"

namespace pyrite {

  Value* CallModel::codegen(Compiler* c) {
    // for debugging
    if (name == "print") {
      Function* func = c->module->getFunction("print");
      std::vector<Value*> argValues;
      argValues.push_back(args->get(0)->codegen(c));
      return c->builder->CreateCall(func, argValues.begin(), argValues.end());
    }

    int argc = args->size();

    Value* func = c->symtab->lookup(name);
    if (!func) throw "'" + name + "' is not defined.";
    func = c->builder->CreateLoad(func);

    Function* callFunc = c->module->getFunction("call");

    std::vector<Value*> argValues;
    argValues.push_back(func);
    argValues.push_back(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), argc));

    if (args) {
      for (unsigned int i = 0; i < argc; i++) {
        argValues.push_back(args->get(i)->codegen(c));
      }
    }

    return c->builder->CreateCall(callFunc, argValues.begin(), argValues.end(), "calltmp");
  }

}
