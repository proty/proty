#ifndef PROTY_COMPILER_HH
#define PROTY_COMPILER_HH

#include "llvm.hh"
#include "ast.hh"

class Compiler {
public:
  llvm::IRBuilder<>* builder;
  llvm::Module* module;
  llvm::ExecutionEngine *executionEngine;
  llvm::FunctionPassManager* fpm;
  llvm::Linker* linker;

public:
  Compiler(std::string);

  llvm::Module* compile(Node*);
};

#endif