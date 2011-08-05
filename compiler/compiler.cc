#include "compiler.hh"
#include "ast.hh"
#include "symtab.hh"
#include "program.hh"
#include "llvm.hh"
#include "error.hh"
#include "config.hh"

using namespace llvm;

Compiler::Compiler(std::string name, bool interactive) {
  builder = new IRBuilder<>(getGlobalContext());
  module = new Module(name, getGlobalContext());
  linker = new Linker(name, module);

  symtab = new SymbolTable();
  toplevel = true;

  debug = false;
  this->interactive = interactive;

  // setup the ExecutionEngine
  InitializeNativeTarget();
  EngineBuilder engineBuilder(module);
  engineBuilder.setEngineKind(EngineKind::JIT);
  engineBuilder.setOptLevel(CodeGenOpt::Aggressive);
  executionEngine = engineBuilder.create();

  // setup the FunctionPassManager
  fpm = new FunctionPassManager(module);
  fpm->add(new TargetData(*executionEngine->getTargetData()));
  fpm->add(createPromoteMemoryToRegisterPass());
  fpm->add(createInstructionCombiningPass());
  fpm->add(createReassociatePass());
  fpm->add(createGVNPass());
  fpm->add(createCFGSimplificationPass());

  // link in the runtime
  loadModule("runtime", false);

  // get the object type which is defined in runtime.bc
  ObjectTy = PointerType::get(module->getTypeByName("struct.Object"), 0);

  // define proty runtime classes
  symtab->enterScope();
  symtab->store("Object", module->getNamedValue("Object_proto"));
  symtab->store("Integer", module->getNamedValue("Integer_proto"));
  symtab->store("Float", module->getNamedValue("Float_proto"));
  symtab->store("String", module->getNamedValue("String_proto"));
  symtab->store("Exception", module->getNamedValue("Exception_proto"));
  symtab->store("Hash", module->getNamedValue("Hash_proto"));
  symtab->store("List", module->getNamedValue("List_proto"));

  Qnil = module->getNamedValue("Qnil");
  symtab->store("nil", Qnil);

  Qtrue = module->getNamedValue("Qtrue");
  symtab->store("true", Qtrue);

  Qfalse = module->getNamedValue("Qfalse");
  symtab->store("false", Qfalse);

  // declare llvm functions
  getDeclaration(module, llvm::Intrinsic::eh_exception);
  getDeclaration(module, llvm::Intrinsic::eh_selector);

  if (!interactive) {
    // create the main function
    std::vector<const Type*> argTypes;
    argTypes.push_back(Type::getInt32Ty(getGlobalContext()));
    argTypes.push_back(PointerType::get(Type::getInt8PtrTy(getGlobalContext()), 0));
    FunctionType* mainTy = FunctionType::get(Type::getInt32Ty(getGlobalContext()), argTypes, false);
    Function* main = Function::Create(mainTy, Function::ExternalLinkage, "main", module);

    BasicBlock* initBB = BasicBlock::Create(getGlobalContext(), "init", main);
    builder->SetInsertPoint(initBB);

    // init the runtime
    Function* runtime_init = module->getFunction("runtime_init");
    builder->CreateCall(runtime_init);

    BasicBlock* mainBB = BasicBlock::Create(getGlobalContext(), "start", main);
    builder->CreateBr(mainBB);
    builder->SetInsertPoint(mainBB);
  }
  else {
    // init the runtime (now)
    Function* runtime_init = module->getFunction("runtime_init");
    void (*init)() = (void (*)())executionEngine->getPointerToFunction(runtime_init);
    init();
  }
}

Compiler::~Compiler() {
  delete symtab;
  delete builder;
  executionEngine->removeModule(module);
  delete executionEngine;
  delete linker;
}

void Compiler::addNode(Node* root) {
  root->codegen(this);
}

void Compiler::run(Node* root) {
  if (!interactive) return;
  FunctionType* funcTy = FunctionType::get(Type::getVoidTy(getGlobalContext()), false);
  Function* func = Function::Create(funcTy, Function::ExternalLinkage, "interactive", module);
  BasicBlock* BB= BasicBlock::Create(getGlobalContext(), "entry", func);
  builder->CreateBr(BB);
  builder->SetInsertPoint(BB);

  root->codegen(this);
  builder->CreateRetVoid();

  void (*f)() = (void (*)())executionEngine->getPointerToFunction(func);
  f();
}

Program* Compiler::getProgram() {
  BasicBlock* currBB = builder->GetInsertBlock();
  if (!currBB->getTerminator()) {
    Value* status = ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0);
    builder->CreateRet(status);
  }

  currBB->getParent()->dump();

  // verify and optimize main
  Function* main = module->getFunction("main");
  verifyFunction(*main);
  fpm->run(*main);

  return new Program(CloneModule(module));
}

void Compiler::loadModule(std::string name, bool init) {
  bool native = false;
  sys::Path path;

  // check local files
  path = sys::Path(name + ".bc");

  if (!path.isBitcodeFile()) {
    path = sys::Path("lib/" + name + ".bc");
  }

  // check installation directory
  if (!path.isBitcodeFile()) {
    path = sys::Path(PREFIX "/lib/proty/" + name + ".bc");
  }

  if (!path.isBitcodeFile()) {
    throw new CompileError("could not find module '" + name + "'");
  }
  else {
    linker->LinkInFile(path, native);
  }

  // add this module to the module list
  modules[name] = path.c_str();

  if (init) {
    // initialize module in main function
    Function* initFunc = module->getFunction(name + "_init");

    if (!interactive) {
      Function* mainFunc = module->getFunction("main");
      IRBuilder<> tmpBuilder(&mainFunc->getEntryBlock(), --mainFunc->getEntryBlock().end());
      tmpBuilder.CreateCall(initFunc);
    }
    else {
      builder->CreateCall(initFunc);
    }
  }
}
