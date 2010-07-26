#ifndef PYRITE_SYMTABLE_HH
#define PYRITE_SYMTABLE_HH

#include <map>

namespace pyrite {

  /// Represents a symboltable.
  /// The SymTable class holds all local
  /// defined variables in a block.
  /// The variable names are saved with
  /// the allocations they are stored in.
  class SymTable {
    public:
      /// Saves a variable name with
      /// an AllocaInst
      /// in the symboltable.
      void set(std::string, AllocaInst*);
      
      /// Get the AllocaInst of an existing 
      /// variable. Returns 0 if the variable
      /// don't exists.
      AllocaInst* get(std::string);

    private:
      std::map<std::string, AllocaInst*> table;
  };

}

#endif
