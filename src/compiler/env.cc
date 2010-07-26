#include "compiler/compiler.hh"
#include "compiler/env.hh"
#include "compiler/symtable.hh"

namespace pyrite {
  
  Env::Env() {

  }
  
  void Env::push() {
    stack.push(new SymTable());
  }
  
  void Env::pop() {
    stack.pop();
  }
  
  void Env::push_namespace(std::string name) {
    ns.push_back(name);
  }
  
  std::string Env::get_namespace() {
    std::string nsstring;
    
    for (unsigned int i=0; i<ns.size(); i++) {
      nsstring += ns[i] + "::";
    }
    return nsstring;
  }
  
  void Env::pop_namespace() {
    ns.pop_back();
  }
  
  void Env::set_class(std::string name) {
    class_name = name;
  }

  bool Env::in_class() {
    return !class_name.empty();
  }

  std::string Env::get_class() {
    return class_name;
  }

  void Env::clear_class() {
    class_name.clear();
  }

  std::string Env::format_function_name(std::string name) {
    std::cout << "function name: " << name << std::endl;
    if (!ns.empty()) {
      return get_namespace() + name;
    }
    else {
      return name;
    }
  }

  std::string Env::format_class_name(std::string name) {
     if (!ns.empty()) {
       return get_namespace() + name;
     }
     else {
       return name;
     }
  }

  SymTable* Env::symtab() {
    return stack.top();
  }
  
}
