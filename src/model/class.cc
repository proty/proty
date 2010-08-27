#include "model/models.hh"

namespace pyrite {

  std::string ClassModel::getName() {
    return name;
  }

  const Type* ClassModel::getType(Compiler* c) {
    return PointerType::get(c->module->getTypeByName(name), 0);
  }

  void ClassModel::addBase(std::string base) {
    bases.push_back(base);
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

    // add parent class types
    std::vector<std::string>::iterator base_it;
    for (base_it = bases.begin(); base_it != bases.end(); base_it++) {
      elts.push_back(c->module->getTypeByName(*base_it));
    }

    // add attributes to the type
    std::map<std::string, TypeModel*>::iterator attr_it;
    for (attr_it = attributes.begin(); attr_it != attributes.end(); attr_it++) {
      elts.push_back(attr_it->second->get(c));
    }

    const Type* classtype = StructType::get(getGlobalContext(), elts);

    cast<OpaqueType>(pholder.get())->refineAbstractTypeTo(classtype);
  }

  void ClassModel::generateMethodPrototypes(Compiler* c) {
    std::vector<FunctionModel*>::iterator mit;
    for (mit = methods.begin(); mit != methods.end(); mit++) {
      (*mit)->generatePrototype(c);
    }

    // create new prototype
    Function* initFunc = c->module->getFunction(name + "::init");
    std::vector<const Type*> newArgs;
    if (initFunc) {
      const FunctionType* Ft = initFunc->getFunctionType();
      FunctionType::param_iterator It;
      for (It = Ft->param_begin(), It++; It != Ft->param_end(); It++) {
        newArgs.push_back((It->get()));
      }
    }
    FunctionType* newFT = FunctionType::get(getType(c), newArgs, false);
    Function::Create(newFT, Function::ExternalLinkage, name + "::new", c->module);

    std::map<std::string, TypeModel*>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++) {
      std::string n = it->first;
      const Type* t = it->second->get(c);

      // create getter prototype
      std::vector<const Type*> getArgs;
      getArgs.push_back(getType(c));
      FunctionType* getFT = FunctionType::get(t, getArgs, false);
      Function::Create(getFT, Function::ExternalLinkage, name + "::get_" + n, c->module);

      // create setter prototype
      std::vector<const Type*>setArgs;
      setArgs.push_back(getType(c));
      setArgs.push_back(t);
      FunctionType* setFT = FunctionType::get((new TypeModel("Nil"))->get(c), setArgs, false);
      Function::Create(setFT, Function::ExternalLinkage, name + "::set_" + n, c->module);
    }
  }

  void ClassModel::generateMethodFunctions(Compiler* c) {
    // backup main basic block
    BasicBlock* MainBB = c->builder->GetInsertBlock();
    BasicBlock* BB;

    // create new function
    {
      Function* F = c->module->getFunction(name + "::new");
      BB = BasicBlock::Create(getGlobalContext(), "entry", F);
      c->builder->SetInsertPoint(BB);
      AllocaInst* Alloca = c->builder->CreateAlloca(getType(c)->getContainedType(0), 0, "self");

      Function* initFunc = c->module->getFunction(name + "::init");
      if (initFunc) {
        std::vector<Value*> argValues;
        argValues.push_back(Alloca);

        Function::arg_iterator It;
        for (It = F->arg_begin(); It != F->arg_end(); It++) {
          AllocaInst* Alloca = c->builder->CreateAlloca(It->getType(), 0, It->getName());
          c->builder->CreateStore(It, Alloca);
          Value* arg = c->builder->CreateLoad(Alloca);
          argValues.push_back(arg);
        }
        c->builder->CreateCall(initFunc, argValues.begin(), argValues.end());
      }

      c->builder->CreateRet(Alloca);
      verifyFunction(*F);
      c->fpm->run(*F);
    }

    std::map<std::string, TypeModel*>::iterator it;
    unsigned int pos = bases.size();
    for (it = attributes.begin(); it != attributes.end(); it++, pos++) {
      std::string n = it->first;
      const Type* t = it->second->get(c);

      // create getter function
      {
        Function* F = c->module->getFunction(name + "::get_" + n);
        BB = BasicBlock::Create(getGlobalContext(), "entry", F);
        c->builder->SetInsertPoint(BB);

        Function::arg_iterator AI = F->arg_begin();
        AllocaInst* Alloca = c->builder->CreateAlloca(getType(c), 0, "self");
        c->builder->CreateStore(AI++, Alloca);

        Value* self = c->builder->CreateLoad(Alloca);
        Value* attr = c->builder->CreateStructGEP(self, pos);

        attr = c->builder->CreateLoad(attr);
        AllocaInst* AttrAlloca = c->builder->CreateAlloca(t, 0, "attr");

        // return the value
        c->builder->CreateStore(attr, AttrAlloca);
        c->builder->CreateRet(c->builder->CreateLoad(AttrAlloca));

        verifyFunction(*F);
        c->fpm->run(*F);
      }

      // create setter function
      {
        Function* F = c->module->getFunction(name + "::set_" + n);
        BB = BasicBlock::Create(getGlobalContext(), "entry", F);
        c->builder->SetInsertPoint(BB);

        Function::arg_iterator AI =   F->arg_begin();
        AllocaInst* SelfAlloca = c->builder->CreateAlloca(getType(c), 0, "self");
        c->builder->CreateStore(AI++, SelfAlloca);

        AllocaInst* ValueAlloca = c->builder->CreateAlloca(t, 0, "value");
        c->builder->CreateStore(AI, ValueAlloca);

        Value* self = c->builder->CreateLoad(SelfAlloca);
        Value* attr = c->builder->CreateStructGEP(self, pos);
        Value* value = c->builder->CreateLoad(ValueAlloca);

        c->builder->CreateStore(value, attr);

        (new ReturnModel(new NilModel()))->codegen(c);

        verifyFunction(*F);
        c->fpm->run(*F);
      }
    }

    // restore main basic block
    c->builder->SetInsertPoint(MainBB);

    std::vector<FunctionModel*>::iterator mit;
    for (mit = methods.begin(); mit != methods.end(); mit++) {
      (*mit)->generateFunction(c);
    }
  }

}
