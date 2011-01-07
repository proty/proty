#include "symtab.hh"

using namespace llvm;

void Scope::set(std::string name, Value* value) {
  variables[name] = value;
}

Value* Scope::get(std::string name) {
  return variables[name];
}

void SymbolTable::enterScope() {
  scopes.push_back(new Scope());
}

void SymbolTable::leaveScope() {
  scopes.pop_back();
}

Value* SymbolTable::lookup(std::string name) {
  Value* v;
  for (int i = scopes.size()-1; i >= 0; i--) {
    if ((v = scopes[i]->get(name))) return v;
  }
  return v;
}

void SymbolTable::store(std::string name, Value* value) {
  scopes.back()->set(name, value);
}