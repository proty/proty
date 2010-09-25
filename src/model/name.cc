#include "model/models.hh"

namespace pyrite {

  std::string NameModel::get_name() {
    return name;
  }

  Value* NameModel::codegen(Compiler* c) {
    Value* v = c->symtab->lookup(name);
    if (!v) throw "Variable '" + name + "' not declared";
    return c->builder->CreateLoad(v, name.c_str());
  }

}
