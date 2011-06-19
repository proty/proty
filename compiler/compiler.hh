#ifndef PROTY_COMPILER_HH
#define PROTY_COMPILER_HH

#include <iostream>

class Node;
class SymbolTable;
class Program;

namespace llvm {
  class ConstantFolder;
  template <bool> class IRBuilderDefaultInserter;
  template <bool, typename, typename> class IRBuilder;
  class Module;
  class ExecutionEngine;
  class FunctionPassManager;
  class Linker;
  class Type;
}

/**
 * Holds the compilation context.
 */

class Compiler {
public:
  llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true> >* builder;
  llvm::Module* module;
  llvm::ExecutionEngine *executionEngine;
  llvm::FunctionPassManager* fpm;
  llvm::Linker* linker;
  const llvm::Type* ObjectTy;
  SymbolTable* symtab;

  bool debug;

public:
  Compiler(std::string, bool=false);

  Program* compile(Node*);
  void loadModule(std::string);
};

#endif
