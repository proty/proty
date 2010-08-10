#include "model/models.hh"

namespace pyrite {
  
  Value* BoolModel::codegen(Compiler* c) {
    CallArgsModel* args = new CallArgsModel();
    args->push(new ValueModel(ConstantInt::get(Type::getInt1Ty(getGlobalContext()), value)));
    CallModel* call = new CallModel("Bool::new", args);
    return call->codegen(c);
  }
  
}