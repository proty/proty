#ifndef PROTY_COMPILER_HH
#define PROTY_COMPILER_HH

#include <iostream>
#include <stack>
#include <map>

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
  class Function;
  class Linker;
  class Type;
  class Value;
}

/**
 * Holds the compilation context.
 */

class Compiler {
private:
  llvm::Function* initFunction;

public:
  llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true> >* builder;
  llvm::Module* module;
  llvm::ExecutionEngine *executionEngine;
  llvm::FunctionPassManager* fpm;
  llvm::Linker* linker;

  /// contains all imported modules
  std::map<std::string, llvm::Module*> modules;

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
  Program* getProgram(bool);
  llvm::Module* loadModule(std::string);

  const llvm::Type* getObjectTy();
  llvm::Value* getBool(bool);
  llvm::Value* getNil();

  llvm::Value* declareExternObject(std::string);
};

#endif
