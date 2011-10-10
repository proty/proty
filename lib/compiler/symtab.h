#ifndef PROTY_SYMTAB_HH
#define PROTY_SYMTAB_HH

#include <iostream>
#include <vector>
#include <map>

namespace llvm { class Value; }

/**
 * Maps variables to their values;
 */

class Scope {
private:
  std::map<std::string, llvm::Value*> variables;

public:
  void set(std::string, llvm::Value*);
  llvm::Value* get(std::string);
};

/**
 * Manages scopes and stores symbols.
 */

class SymbolTable {
private:
  std::vector<Scope*> scopes;

public:
  void enterScope();
  void leaveScope();

  llvm::Value* lookup(std::string);
  void store(std::string, llvm::Value*);
};

#endif
