#include "model/models.hh"

namespace pyrite {
  
  Value* IntegerModel::codegen(Compiler* c) {
    Value* integer = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), value);
    CallArgsModel* args = new CallArgsModel();
    args->push(new ValueModel(integer));
    CallModel* call = new CallModel("Integer::new", args);
    return call->codegen(c);
  }

}
