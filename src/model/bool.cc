#include "model/models.hh"

namespace pyrite {
  
  Value* BoolModel::codegen(Compiler* c) {
    CallArgsModel* args = new CallArgsModel();
    CallModel* call = new CallModel("Bool::new", args);
    return call->codegen(c);
  }
  
}