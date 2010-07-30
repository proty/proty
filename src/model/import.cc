#include "model/models.hh"

namespace pyrite {
  
  Value* ImportModel::codegen(Compiler* c) {
    c->link_pyrite(name + ".pr");
    return 0;
  }

}