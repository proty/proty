#ifndef PROTY_PROGRAM_HH
#define PROTY_PROGRAM_HH

#include <iostream>

namespace llvm { class Module; }

/**
 * A compiled program.
 */

class Program {
private:
  llvm::Module* module;

public:
  Program(llvm::Module* module) : module(module) {}

  void dump();
  void writeToFile(std::string);
  void run();
};

#endif
