#include "model/models.hh"

namespace pyrite {

  void ClassModel::addBase(std::string base) {
    bases.push_back(base);
  }

  Value* ClassModel::codegen(Compiler* c) {
    return 0;
  }

}
