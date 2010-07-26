#include "model/models.hh"

namespace pyrite {
  
  Value* DoubleModel::codegen(Compiler* c) {
    return ConstantFP::get(getGlobalContext(), APFloat(value));
  }
  
}