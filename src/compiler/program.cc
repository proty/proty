#include "compiler/models.hh"

namespace pyrite {

  void ProgramModel::generate(Compiler* c) {
    codegen(c);
  }

}