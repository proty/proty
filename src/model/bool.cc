#include "model/models.hh"

namespace pyrite {
  
  Value* BoolModel::codegen(Compiler* c) {
    return ConstantInt::get(Type::getInt1Ty(getGlobalContext()), value);
  }
  
}