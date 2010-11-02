#include "compiler/compiler.hh"
#include "model/models.hh"
#include "parser/parser.hh"
#include "config.hh"
#include <cstdlib>

namespace pyrite {

  Compiler::Compiler(std::string name) {
    program = new ProgramModel();
    symtab = new SymbolTable();

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

    // Fill in search paths
    search_paths.push_back("./lib");
    search_paths.push_back(BUILD_PREFIX "/lib/pyrite");

    // link in the virtual machine
    linkLLVMbc("vm.bc");
  }

  Module* Compiler::compile(ProgramModel* root, bool main) {
    std::vector<const Type*> argTypes;
    FunctionType *FT = FunctionType::get(Type::getVoidTy(getGlobalContext()), argTypes, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "main", module);

    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    builder->SetInsertPoint(BB);

    try {
      symtab->enterScope();
      program->codegen(this);
      symtab->leaveScope();
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
    p->parse();
    
    /// @todo: link in another pyrite program
  }

  void Compiler::linkLLVMbc(std::string file) {
    bool native = false;

    sys::Path path;
    std::vector<std::string>::iterator it;
    for (it = search_paths.begin(); it != search_paths.end(); it++) {
      path = sys::Path(*it + "/" + file);
      if (path.exists()) break;
      else path.clear();
    }
    if (path.empty()) error(file + " not found.");

    linker->LinkInFile(path, native);
  }

  void Compiler::error(std::string msg) {
    std::cout << "pyrite: Error: " << msg << std::endl;
    std::exit(1);
  }

  void Compiler::warning(std::string msg) {
    std::cout << "pyrite: Warning: " << msg << std::endl;
  }

}
