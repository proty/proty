#ifndef PYRITE_ENV_HH
#define PYRITE_ENV_HH

#include <stack>
#include <vector>
#include <iostream>
#include "compiler/symtable.hh"

namespace pyrite {

  /// Handles the Environment
  /// while compiling.
  class Env {
    public:
      Env();

      /// Pushes a new symtable on the stack.
      void push();
      /// Pops a symtable from the stack.
      void pop();
      /// Get the symtable from the top of the stack.
      SymTable* symtab();

      /// Add a namespace (e.g. for a module).
      void push_namespace(std::string);
      /// Build a string with all namespaces.
      std::string get_namespace();
      /// Removes a namespace.
      void pop_namespace();

      void set_class(std::string);
      bool in_class();
      std::string get_class();
      void clear_class();

      std::string format_function_name(std::string);
      std::string format_class_name(std::string);
    
    private:
      std::stack<SymTable*> stack;
      std::vector<std::string> ns;
      std::string class_name;
  };
  
}

#endif
