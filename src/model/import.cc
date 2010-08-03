#include "model/models.hh"

namespace pyrite {
  
  Value* ImportModel::codegen(Compiler* c) {
    c->linkPyrite(name + ".pr");
    return 0;
  }

}