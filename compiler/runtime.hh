#ifndef PROTY_RUNTIME
#define PROTY_RUNTIME

/**
 * Manages functions and types that are needed to use the runtime
 * library.
 */

class Runtime {
public:
  static void declareTypes(Compiler* c);
  static void declareFunctions(Compiler* c);

private:
  static llvm::Value* externObject(Compiler*, std::string);
};

#endif
