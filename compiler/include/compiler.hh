#ifndef PROTY_COMPILER_HH
#define PROTY_COMPILER_HH

#include "llvm.hh"
#include "ast.hh"
#include "symtab.hh"

class Compiler {
public:
  llvm::IRBuilder<>* builder;
  llvm::Module* module;
  llvm::ExecutionEngine *executionEngine;
  llvm::FunctionPassManager* fpm;
  llvm::Linker* linker;
  const llvm::Type* ObjectTy;
  SymbolTable* symtab;

public:
  Compiler(std::string);

  llvm::Module* compile(Node*);
};

#endif