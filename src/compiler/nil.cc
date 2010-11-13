#include "compiler/models.hh"

namespace pyrite {

  Value* NilModel::codegen(Compiler* c) {
    CallArgsModel* args = new CallArgsModel();
    CallModel* call = new CallModel("Nil::new", args);
    return call->codegen(c);
  }

}