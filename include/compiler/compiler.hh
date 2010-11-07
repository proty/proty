#ifndef PYRITE_COMPILER_HH
#define PYRITE_COMPILER_HH

#include "llvm.hh"
#include "symtab.hh"
#include <vector>

namespace pyrite {

  class ProgramModel;

  /// A class to handle the compilation
  /// of a ProgramModel to a LLVM Module
  class Compiler {
  private:
    std::vector<std::string> search_paths;

  public:
    Compiler(std::string);

    /// Compiles a ProgramModel to a LLVM Module.
    /// @returns the compiled code as a LLVM Module
    Module* compile(ProgramModel*, bool=true);
    ProgramModel* program;
    SymbolTable* symtab;

    /// Pyrite Object Type
    const Type* ObjectTy;
    const Type* FunctionPtr;

    /// LLVM Classes
    IRBuilder<>* builder;
    Module* module;
    ExecutionEngine *executionEngine;
    FunctionPassManager* fpm;
    Linker* linker;

    /// Link in pyrite file
    void linkPyrite(std::string);
    /// Link in a llvm bytecode file
    void linkLLVMbc(std::string);

    /// Prints out an error messages
    /// and stops the program.
    void error(std::string);

    /// Prints out a warning.
    void warning(std::string);
  };

}

#endif  
