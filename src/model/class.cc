#include "model/models.hh"

namespace pyrite {

  std::string ClassModel::getName() {
    return name;
  }

  const Type* ClassModel::getType(Compiler* c) {
    return PointerType::get(c->module->getTypeByName(name), 0);
  }

  void ClassModel::addMethod(FunctionModel* function) {
    function->methodOf(this);
    methods.push_back(function);
  }

  void ClassModel::addAttribute(std::string n, TypeModel* t) {
    attributes[n] = t;
  }

  void ClassModel::generatePlaceholder(Compiler* c) {
    pholder = OpaqueType::get(getGlobalContext());
    c->module->addTypeName(name, pholder);
  }

  void ClassModel::generateType(Compiler* c) {
    std::vector<const Type*> elts;

    // add attributes to the type
    std::map<std::string, TypeModel*>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++) {
      elts.push_back(it->second->get(c));
    }

    const Type* classtype = StructType::get(getGlobalContext(), elts);

    cast<OpaqueType>(pholder.get())->refineAbstractTypeTo(classtype);
  }

  void ClassModel::generateMethodPrototypes(Compiler* c) {
    std::map<std::string, TypeModel*>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++) {
      std::string n = it->first;
      const Type* t = it->second->get(c);

      // create getter prototype
      std::vector<const Type*> getArgs;
      FunctionType* getFT = FunctionType::get(t, getArgs, false);
      Function* getF = Function::Create(getFT, Function::ExternalLinkage, name + ".get_" + n, c->module);
      getF->addAttribute(0, Attribute::AlwaysInline);

      // create setter prototype
      std::vector<const Type*>setArgs;
      setArgs.push_back(t);
      FunctionType* setFT = FunctionType::get(Type::getVoidTy(getGlobalContext()), setArgs, false);
      Function* setF = Function::Create(setFT, Function::ExternalLinkage, name + ".set_" + n, c->module);
      setF->addAttribute(0, Attribute::AlwaysInline);
    }

    std::vector<FunctionModel*>::iterator mit;
    for (mit = methods.begin(); mit != methods.end(); ++mit) {
      (*mit)->generatePrototype(c);
    }
  }

  void ClassModel::generateMethodFunctions(Compiler* c) {
    std::map<std::string, TypeModel*>::iterator it;

    // backup main basic block
    BasicBlock* MainBB = c->builder->GetInsertBlock();
    BasicBlock* BB;

    for (it = attributes.begin(); it != attributes.end(); it++) {
      std::string n = it->first;
      const Type* t = it->second->get(c);

      // create getter function
      Function* getF = c->module->getFunction(name + ".get_" + n);
      BB = BasicBlock::Create(getGlobalContext(), "entry", getF);
      c->builder->SetInsertPoint(BB);

      // create setter function
      Function* setF = c->module->getFunction(name + ".set_" + n);
      BB = BasicBlock::Create(getGlobalContext(), "entry", setF);
      c->builder->SetInsertPoint(BB);

      Function::arg_iterator AI = setF->arg_begin();
      AllocaInst* Alloca = c->builder->CreateAlloca(t, 0, "value");
      c->builder->CreateStore(AI, Alloca);
    }

    // restore main basic block
    c->builder->SetInsertPoint(MainBB);

    std::vector<FunctionModel*>::iterator mit;
    for (mit = methods.begin(); mit != methods.end(); ++mit) {
      (*mit)->generateFunction(c);
    }
  }

}
