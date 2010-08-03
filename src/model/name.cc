#include "model/models.hh"

namespace pyrite {

  std::string NameModel::get_name() {
    return name;
  }

  Value* NameModel::codegen(Compiler* c) {
    BasicBlock* BB = c->builder->GetInsertBlock();
    ValueSymbolTable* symtab = BB->getValueSymbolTable();
    Value* v = symtab->lookup(name);
    if (!v) throw "Variable '" + name + "' not declared";
    return c->builder->CreateLoad(v, name.c_str());
  }

}
