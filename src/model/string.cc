#include "model/models.hh"

namespace pyrite {
  
  Value* StringModel::codegen(Compiler* c) {
    size_t pos = 0;
    while((pos = value.find("\\n", pos)) != std::string::npos) {
      value.replace(pos, 2, "\n");
      pos += 1;
    }

    Value* string = c->builder->CreateGlobalStringPtr(value.c_str(), ".str");

    CallArgsModel* args = new CallArgsModel();
    args->push(new ValueModel(string));
    CallModel* call = new CallModel("String::new", args);
    return call->codegen(c);
  }

}
