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
 * Manages compiliation and holds the compilation context.
 */

class Compiler {
private:
  /// the init function of the current Module
  llvm::Function* initFunction;

public:
  llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true> >* builder;
  llvm::Module* module;
  llvm::ExecutionEngine *executionEngine;
  llvm::FunctionPassManager* fpm;
  llvm::Linker* linker;

  /// contains all imported modules
  std::map<std::string, llvm::Module*> modules;

  /// contains the catch blocks for unwinding
  std::stack<llvm::BasicBlock*> unwind;

  /// contains all symbols and scopes
  SymbolTable* symtab;

  /// true if we're not inside a function definition
  bool toplevel;

  /// debug flag to include debugging symbols
  bool debug;

  /// true if this an interactive session
  bool interactive;

public:
  /**
   * Creates a new Compiler object. Initializes all classes that are
   * needed for compilation.
   *
   * @param name Name for the Module
   * @param interactive True when the Compiler is used interactive.
   */
  Compiler(std::string name, bool interactive=false);

  ~Compiler();

  /**
   * Compile the given Node.
   *
   * @param root The Node to compile
   */
  void compile(Node* root);

  /**
   * Compile the given Node and run it with the LLVM Jit.
   *
   * @param root The Node to compile
   */
  void run(Node* root);

  /**
   * Completes the Module to an standalone module or to an linkable
   * module.
   *
   * @param standalone If true, a main function will be generated
   * @returns The completed program
   */
  Program* getProgram(bool standalone);

  /**
   * Searches for a module with the given name. If the module is
   * found, it gets inserted into the module map, if not, a
   * CompileError is thrown.
   *
   * @param name The module name
   * @returns The loaded module
   */
  llvm::Module* loadModule(std::string name);

  /**
   * Searches the Module for the object type (struct.Object).
   *
   * @returns The object type
   */
  const llvm::Type* getObjectTy();

  /**
   * Searches the Module for the true/false object.
   *
   * @param value true for Qtrue, false for Qfalse
   * @returns The GlobalVariable for Qtrue or Qfalse
   */
  llvm::Value* getBool(bool value);

  /**
   * Searches the Module for the nil object.
   *
   * @returns The GlobalVariable for Qnil
   */
  llvm::Value* getNil();

  /**
   * Declares a GlobalVariable with the object type.
   *
   * @param name Name for the GlobalVariable
   * @returns The declared GlobalVariable
   */
  llvm::Value* declareExternObject(std::string name);
};

#endif
