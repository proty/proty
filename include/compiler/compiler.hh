#ifndef PYRITE_COMPILER_HH
#define PYRITE_COMPILER_HH

#include "llvm.hh"
#include "compiler/env.hh"
#include <vector>

namespace pyrite {

  class BlockModel;


  /// A class to handle the compilation
  /// of a BlockModel to a LLVM Module
  class Compiler {
  public:
    Compiler(std::string);

    /// Compiles a BlockModel to a LLVM Module.
    /// @returns the compiled code as a LLVM Module
    Module* compile(BlockModel*);

    /// LLVM Classes
    IRBuilder<>* builder;
    Module* module;
    ExecutionEngine *executionEngine;
    FunctionPassManager* fpm;
    Linker* linker;

    /// The environment.
    Env* env;
    
    /// Link in pyrite file
    void link_pyrite(std::string);
    /// Link in a llvm bytecode file
    void link_llvm_bc(std::string);

    /// Prints out an error messages
    /// and stops the program.
    void error(std::string);

    /// Prints out a warning.
    void warning(std::string);
  };

}

#endif  
