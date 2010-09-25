#ifndef PYRITE_SYMTAB_HH
#define PYRITE_SYMTAB_HH

#include "llvm.hh"
#include <vector>
#include <map>

namespace pyrite {

  class Scope {
  private:
    std::map<std::string, Value*> variables;

  public:
    void set(std::string, Value*);
    Value* get(std::string);
  };

  class SymbolTable {
  private:
    std::vector<Scope*> scopes;

  public:
    void enterScope();
    void leaveScope();

    Value* lookup(std::string);
    void store(std::string, Value*);
  };

}

#endif
