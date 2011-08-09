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
    BasicBlock* entryBB = BasicBlock::Create(getGlobalContext(), "entry", initFunction);
    builder->SetInsertPoint(entryBB);
  }
  else {
    loadModule("runtime");

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

void Compiler::compile(Node* root) {
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

Program* Compiler::getProgram(bool standalone) {
  // terminate the init function for the current module
  builder->CreateRetVoid();

  // verify and optimize init function
  verifyFunction(*initFunction);
  fpm->run(*initFunction);

  if (standalone) {
    loadModule("runtime");

    // create a main function
    std::vector<const Type*> argTypes;
    argTypes.push_back(Type::getInt32Ty(getGlobalContext()));
    argTypes.push_back(PointerType::get(Type::getInt8PtrTy(getGlobalContext()), 0));

    const Type* retTy = Type::getInt32Ty(getGlobalContext());
    FunctionType* mainTy = FunctionType::get(retTy, argTypes, false);
    Function* main = Function::Create(mainTy, Function::ExternalLinkage, "main", module);

    BasicBlock* BB = BasicBlock::Create(getGlobalContext(), "entry", main);
    builder->SetInsertPoint(BB);

    // link in all modules
    std::map<std::string, Module*>::iterator it;
    for (it = modules.begin(); it != modules.end(); it++) {
      linker->LinkInModule((*it).second);
    }


    // make sure that the runtime is initialized first
    Function* runtime_init = module->getFunction("runtime_init");
    builder->CreateCall(runtime_init);
    modules.erase("runtime");

    // load and initialize depended modules
    Module::lib_iterator depmod_it;
    for (depmod_it = module->lib_begin(); depmod_it != module->lib_end(); depmod_it++) {
      std::string depmod = (*depmod_it);
      if (!depmod.compare(0, 3, "pr:")) {
        std::string moduleName = depmod.substr(3);
        Module* m = loadModule(moduleName);
        linker->LinkInModule(m);

        Function* moduleInit = module->getFunction(moduleName + "_init");
        builder->CreateCall(moduleInit);
        modules.erase(moduleName);
      }
    }

    // call the initializers for all remaining modules
    for (it = modules.begin(); it != modules.end(); it++) {
      std::string moduleName = (*it).first;
      Function* moduleInit = module->getFunction(moduleName + "_init");
      builder->CreateCall(moduleInit);
    }

    // call the current module's init function
    builder->CreateCall(initFunction);

    // return exit status 0
    builder->CreateRet(ConstantInt::get(Type::getInt32Ty(getGlobalContext()), 0));
  }
  // if this program is not standalone
  else {
    std::map<std::string, Module*>::iterator it;

    // add dependencies for all modules
    for (it = modules.begin(); it != modules.end(); it++) {
      module->addLibrary("pr:" + (*it).first);
    }
  }

  return new Program(CloneModule(module));
}

Module* Compiler::loadModule(std::string name) {
  // check if the module is already loaded, if not, load it!
  std::map<std::string, Module*>::iterator it = modules.find(name);
  if (it != modules.end()) {
    return (*it).second;
  }

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

  OwningPtr<MemoryBuffer> Buffer;
  MemoryBuffer::getFile(path.c_str(), Buffer);
  Module* mod = ParseBitcodeFile(Buffer.get(), getGlobalContext());

  // add this module to the module list
  modules[name] = mod;

  if (interactive) {
    linker->LinkInModule(mod);
    Function* initFunc = module->getFunction(name + "_init");
    builder->CreateCall(initFunc);
  }

  return mod;
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

Value* Compiler::declareExternObject(std::string name) {
  return new GlobalVariable(*module, getObjectTy(), false,
                            GlobalValue::ExternalWeakLinkage,
                            UndefValue::get(getObjectTy()), name);
}
