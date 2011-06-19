#include "compiler.hh"
#include "ast.hh"
#include "symtab.hh"
#include "program.hh"
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

Compiler::~Compiler() {
  delete symtab;
  delete builder;
  executionEngine->removeModule(module);
  delete executionEngine;
  delete linker;
}

Program* Compiler::compile(Node* root) {
  // create the main function
  std::vector<const Type*> argTypes;
  argTypes.push_back(Type::getInt32Ty(getGlobalContext()));
  argTypes.push_back(PointerType::get(Type::getInt8PtrTy(getGlobalContext()), 0));
  FunctionType *funcTy = FunctionType::get(Type::getInt32Ty(getGlobalContext()), argTypes, false);
  Function *func = Function::Create(funcTy, Function::ExternalLinkage, "main", module);

  BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", func);
  builder->SetInsertPoint(BB);

  // init the runtime
  Function* runtime_init = module->getFunction("runtime_init");
  builder->CreateCall(runtime_init);

  symtab->enterScope();
  root->codegen(this);
  symtab->leaveScope();

  // return a status code
  Value* status = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0);
  builder->CreateRet(status);

  verifyFunction(*func);
  fpm->run(*func);

  return new Program(CloneModule(module));
}

void Compiler::loadModule(std::string name) {
  bool native = false;
  sys::Path path = sys::Path("lib/" + name + ".bc");
  linker->LinkInFile(path, native);
}
