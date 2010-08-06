#include "model/models.hh"

namespace pyrite {

  Value* AttributeModel::codegen(Compiler* c) {
    CallArgsModel* args = new CallArgsModel();

    std::string n;
    if (value) {
      args->push(value);
      n = "set_" + name;
    }
    else {
      n = "get_" + name;
    }

    MethodCallModel* call = new MethodCallModel(instance, n, args);
    return call->codegen(c);
  }

}
