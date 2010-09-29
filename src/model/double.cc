#include "model/models.hh"

namespace pyrite {
  
  Value* DoubleModel::codegen(Compiler* c) {
    Value* doubleV = ConstantFP::get(getGlobalContext(), APFloat(value));

    /// @todo: implement complex double generation
    return doubleV;
  }
  
}
