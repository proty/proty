#ifndef PROTY_COMPILER_HH
#define PROTY_COMPILER_HH

#include <iostream>
#include <stack>

class Node;
class SymbolTable;
class Program;

namespace llvm {
  class ConstantFolder;
  template <bool> class IRBuilderDefaultInserter;
  template <bool, typename, typename> class IRBuilder;
  class BasicBlock;
  class Module;
  class ExecutionEngine;
  class FunctionPassManager;
  class Linker;
  class Type;
  class Value;
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
  llvm::Value* Qnil;
  llvm::Value* Qtrue;
  llvm::Value* Qfalse;

  std::stack<llvm::BasicBlock*> unwind;
  SymbolTable* symtab;
  bool toplevel;

  bool debug;
  bool interactive;

public:
  Compiler(std::string, bool=false);
  ~Compiler();

  void addNode(Node*);
  void run(Node*);
  Program* getProgram();
  void loadModule(std::string);
};

#endif
