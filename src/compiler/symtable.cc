#include "compiler/compiler.hh"
#include "compiler/symtable.hh"

namespace pyrite {
  
  void SymTable::set(std::string name, AllocaInst* alloca) {
    table[name] = alloca;
  }
  
  AllocaInst* SymTable::get(std::string name) {
    std::map<std::string, AllocaInst*>::iterator found = table.find(name);
    if (found != table.end()) {
      return (*found).second;
    }
    else {
      return 0;
    }
  }
  
}
