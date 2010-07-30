#include "compiler/compiler.hh"
#include "model/models.hh"
#include "parser/parser.hh"
#include <cstdlib>

namespace pyrite {

  Compiler::Compiler(std::string name) {
    env = new Env();
    
    // Initialize LLVM Classes
    builder = new IRBuilder<>(getGlobalContext());
    module = new Module(name, getGlobalContext());
    linker = new Linker(name, module);
    
    // Setup the ExecutionEngine
    InitializeNativeTarget();
    EngineBuilder builder(module);
    builder.setEngineKind(EngineKind::JIT);
    builder.setOptLevel(CodeGenOpt::Aggressive);
    executionEngine = builder.create();
    
    // Setup the FunctionPassManager
    fpm = new FunctionPassManager(module);
    fpm->add(new TargetData(*executionEngine->getTargetData()));
    fpm->add(createPromoteMemoryToRegisterPass());
    fpm->add(createInstructionCombiningPass());
    fpm->add(createReassociatePass());
    fpm->add(createGVNPass());
    fpm->add(createCFGSimplificationPass());
    
    module->addTypeName("prim_int", Type::getInt32Ty(getGlobalContext()));
    module->addTypeName("prim_double", Type::getDoubleTy(getGlobalContext()));
    
    link_pyrite("core/object.pr");
    //link_pyrite("core/type.pr");
    //link_pyrite("core/integer.pr");
  }

  Module* Compiler::compile(BlockModel* root) {
    std::vector<const Type*> argTypes;
    FunctionType *FT = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", module);
    
    env->push();
    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    builder->SetInsertPoint(BB);
    
    try {
      for (unsigned int i = 0; i < root->size(); i++) {
        root->get(i)->codegen(this);
      }
    }
    catch(std::string s) {
      error(s);
    }
    catch(const char* s) {
      error(s);
    }
    
    env->pop();
    builder->CreateRetVoid();
    
    verifyFunction(*F);
    fpm->run(*F);
    
    return module;
  }

  void Compiler::link_pyrite(std::string file) {
    std::cout << "link in " << file << std::endl;
    Tokenizer* t = new Tokenizer("lib/" + file);

    Parser* p = new Parser(t);
    BlockModel* b = p->parse();

    Module* m = compile(b);
    
    // Remove main function
    m->getFunction("main")->removeFromParent();
    
    linker->LinkInModule(m);
  }

  void Compiler::link_llvm_bc(std::string file) {
    bool native = false;
    linker->LinkInFile(sys::Path("lib/" + file), native);
  }

  void Compiler::error(std::string msg) {
    std::cout << "pyrite: Error: " << msg << std::endl;
    std::exit(1);
  }

  void Compiler::warning(std::string msg) {
    std::cout << "pyrite: Warning: " << msg << std::endl;
  }

}
