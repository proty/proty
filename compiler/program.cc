#include "program.hh"
#include "llvm.hh"

using namespace llvm;

void Program::dump() {
  module->dump();
}

void Program::writeToFile(std::string file) {
  std::string error;
  raw_fd_ostream out(file.c_str(), error);
  WriteBitcodeToFile(module, out);
}

void Program::run() {
  InitializeNativeTarget();
  EngineBuilder engineBuilder(module);
  engineBuilder.setEngineKind(EngineKind::JIT);
  engineBuilder.setOptLevel(CodeGenOpt::Aggressive);
  ExecutionEngine* executionEngine = engineBuilder.create();

  std::vector<llvm::GenericValue> args;
  Function* main = executionEngine->FindFunctionNamed("main");
  executionEngine->runStaticConstructorsDestructors(false);
  executionEngine->runFunction(main, args);
}

