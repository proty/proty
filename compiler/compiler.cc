#include "compiler.hh"
#include "ast.hh"
#include "llvm.hh"

using namespace llvm;

Compiler::Compiler(std::string name, bool debug) {
  symtab = new SymbolTable();
  builder = new IRBuilder<>(getGlobalContext());
  module = new Module(name, getGlobalContext());
  linker = new Linker(name, module);

  this->debug = debug;

  // Setup the ExecutionEngine
  InitializeNativeTarget();
  EngineBuilder engineBuilder(module);
  engineBuilder.setEngineKind(EngineKind::JIT);
  engineBuilder.setOptLevel(CodeGenOpt::Aggressive);
  executionEngine = engineBuilder.create();

  // Setup the FunctionPassManager
  fpm = new FunctionPassManager(module);
  fpm->add(new TargetData(*executionEngine->getTargetData()));
  fpm->add(createPromoteMemoryToRegisterPass());
  fpm->add(createInstructionCombiningPass());
  fpm->add(createReassociatePass());
  fpm->add(createGVNPass());
  fpm->add(createCFGSimplificationPass());

  // Link in the VM
  bool native = false;
  linker->LinkInFile(sys::Path("lib/runtime.bc"), native);

  // Get the object type which is defined in runtime.bc
  ObjectTy = PointerType::get(module->getTypeByName("struct.Object"), 0);
}

Module* Compiler::compile(Node* root) {
  // Create the main function
  std::vector<const Type*> argTypes;
  FunctionType *funcTy = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
  Function *func = Function::Create(funcTy, Function::ExternalLinkage, "main", module);


  BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", func);
  builder->SetInsertPoint(BB);

  // Init the runtime
  Function* runtime_init = module->getFunction("runtime_init");
  builder->CreateCall(runtime_init);


  symtab->enterScope();
  root->codegen(this);
  symtab->leaveScope();

  builder->CreateRetVoid();
  verifyFunction(*func);
  fpm->run(*func);

  return module;
}

void Compiler::loadModule(std::string name) {
  bool native = false;
  sys::Path path = sys::Path("lib/" + name + ".bc");
  linker->LinkInFile(path, native);
}
