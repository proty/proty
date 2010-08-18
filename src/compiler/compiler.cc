#include "compiler/compiler.hh"
#include "model/models.hh"
#include "parser/parser.hh"
#include <cstdlib>

namespace pyrite {

  Compiler::Compiler(std::string name) {
    program = new ProgramModel();

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

    linkPyrite("core/object.pr");
    linkPyrite("core/type.pr");
    linkPyrite("core/string.pr");
    linkPyrite("core/bool.pr");
    linkPyrite("core/nil.pr");
    linkPyrite("core/integer.pr");
    linkPyrite("core/io.pr");
  }

  Module* Compiler::compile(ProgramModel* root, bool main) {
    std::vector<const Type*> argTypes;
    FunctionType *FT = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", module);

    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    builder->SetInsertPoint(BB);

    program->mergeIn(root, main);

    try {
      program->generate(this);
    }
    catch(std::string s) {
      error(s);
    }
    catch(const char* s) {
      error(s);
    }

    builder->CreateRetVoid();

    verifyFunction(*F);
    fpm->run(*F);

    return module;
  }

  void Compiler::linkPyrite(std::string file) {
    Tokenizer* t = new Tokenizer("lib/" + file);

    Parser* p = new Parser(t);
    ProgramModel* pm = p->parse();

    program->mergeIn(pm);
  }

  void Compiler::linkLLVMbc(std::string file) {
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
