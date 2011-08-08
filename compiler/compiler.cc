#include "compiler.hh"
#include "ast.hh"
#include "symtab.hh"
#include "runtime.hh"
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

  symtab->enterScope();

  Runtime::declareTypes(this);

  if (!interactive) {
    Runtime::declareFunctions(this);

    // create the init  function
    FunctionType* initTy = FunctionType::get(Type::getVoidTy(getGlobalContext()), false);
    initFunction = Function::Create(initTy, Function::ExternalLinkage, name + "_init", module);
    BasicBlock* initBB = BasicBlock::Create(getGlobalContext(), "init", initFunction);
    builder->SetInsertPoint(initBB);

    // init the runtime
    Function* runtime_init = module->getFunction("runtime_init");
    builder->CreateCall(runtime_init);

    BasicBlock* mainBB = BasicBlock::Create(getGlobalContext(), "start", initFunction);
    builder->CreateBr(mainBB);
    builder->SetInsertPoint(mainBB);
  }
  else {
    loadModule("runtime", false);

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
  BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", func);
  builder->CreateBr(BB);
  builder->SetInsertPoint(BB);

  root->codegen(this);
  builder->CreateRetVoid();

  void (*f)() = (void (*)())executionEngine->getPointerToFunction(func);
  f();
}

Program* Compiler::getProgram() {
  builder->CreateRetVoid();

  // verify and optimize init function
  verifyFunction(*initFunction);
  fpm->run(*initFunction);

  // create main function
  std::vector<const Type*> argTypes;
  argTypes.push_back(Type::getInt32Ty(getGlobalContext()));
  argTypes.push_back(PointerType::get(Type::getInt8PtrTy(getGlobalContext()), 0));
  FunctionType* mainTy = FunctionType::get(Type::getInt32Ty(getGlobalContext()), argTypes, false);
  Function* main = Function::Create(mainTy, Function::ExternalWeakLinkage, "main", module);

  BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", main);
  builder->SetInsertPoint(BB);
  builder->CreateCall(initFunction);
  builder->CreateRet(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0));

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
      IRBuilder<> tmpBuilder(&initFunction->getEntryBlock(), --initFunction->getEntryBlock().end());
      tmpBuilder.CreateCall(initFunc);
    }
    else {
      builder->CreateCall(initFunc);
    }
  }
}

const Type* Compiler::getObjectTy() {
  return module->getTypeByName("struct.Object")->getPointerTo(0);
}

Value* Compiler::getBool(bool value) {
  return module->getNamedValue(value ? "Qtrue" : "Qfalse");
}

Value* Compiler::getNil() {
  return module->getNamedValue("Qnil");
}
