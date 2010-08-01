#include "model/models.hh"

namespace pyrite {

  void ClassModel::add_method(FunctionModel* function) {
    methods.push_back(function);
  }

  void ClassModel::add_attribute(std::string n, TypeModel* t) {
    attributes[n] = t;
  }

  Value* ClassModel::codegen(Compiler* c) {
    name = c->env->format_class_name(name);
    c->env->set_class(name);

    std::vector<const Type*> elts;

    // add attributes to the type
    std::map<std::string, TypeModel*>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++) {
      elts.push_back(it->second->get(c));
    }

    const Type* classtype = StructType::get(getGlobalContext(), elts);
    c->module->addTypeName(name, classtype);


    // backup main basic block
    BasicBlock* MainBB = c->builder->GetInsertBlock();
    BasicBlock* BB;

    // Generate setter and getter methods
    for (it = attributes.begin(); it != attributes.end(); it++) {
      std::string n = it->first;
      const Type* t = it->second->get(c);

      // create getter method
      std::vector<const Type*> getArgs;
      FunctionType* getFT = FunctionType::get(t, getArgs, false);
      Function* getF = Function::Create(getFT, Function::ExternalLinkage, name + ".get_" + n, c->module);
      getF->addAttribute(0, Attribute::AlwaysInline);

      BB = BasicBlock::Create(getGlobalContext(), "entry", getF);
      c->builder->SetInsertPoint(BB);

      // create setter method
      std::vector<const Type*>setArgs;
      setArgs.push_back(t);
      FunctionType* setFT = FunctionType::get(Type::getVoidTy(getGlobalContext()), setArgs, false);
      Function* setF = Function::Create(setFT, Function::ExternalLinkage, name + ".set_" + n, c->module);
      setF->addAttribute(0, Attribute::AlwaysInline);

      BB = BasicBlock::Create(getGlobalContext(), "entry", setF);
      c->builder->SetInsertPoint(BB);

      Function::arg_iterator AI = setF->arg_begin();
      AllocaInst* Alloca = c->builder->CreateAlloca(t, 0, "value");
      c->builder->CreateStore(AI, Alloca);
    }

    // restore main basic block
    c->builder->SetInsertPoint(MainBB);

    // generate the methods
    for (unsigned int i = 0; i < methods.size(); i++) {
      methods.at(i)->codegen(c);
    }

    c->env->clear_class();
    return 0;
  }

}
