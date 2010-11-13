#include "compiler/models.hh"

namespace pyrite {

  void ClassModel::setBase(std::string base) {
    this->base = base;
  }

  void ClassModel::addMethod(FunctionModel* method) {
    methods.push_back(method);
  }

  void ClassModel::addAttribute(std::string key, ExprModel* val) {
    attributes[key] = val;
  }

  std::string ClassModel::getName() {
    return name;
  }

  Value* ClassModel::codegen(Compiler* c) {
    Function* newclassFunc = c->module->getFunction("newclass");
    ValueModel* classObjModel = new ValueModel(c->builder->CreateCall(newclassFunc, name));

    AssignModel* assignment = new AssignModel(name, classObjModel);
    Value* classObj = c->builder->CreateLoad(assignment->codegen(c));

    for (int i = 0; i < methods.size(); i++) {
      FunctionModel* funcModel = methods.at(i);
      funcModel->setMethodOf(this);

      Value* f = c->builder->CreateLoad(methods.at(i)->codegen(c));
      Value* n = (new StringModel(name))->codegen(c);

      Function* setattrFunc = c->module->getFunction("setattr");
      c->builder->CreateCall3(setattrFunc, classObj, n, f);
    }

    std::map<std::string, ExprModel*>::iterator it;
    for (it = attributes.begin(); it != attributes.end(); it++) {
      Value* val = it->second->codegen(c);

      Value* key = (new StringModel(it->first))->codegen(c);

      Function* setattrFunc = c->module->getFunction("setattr");
      c->builder->CreateCall3(setattrFunc, classObj, key, val);
    }

    return 0;
  }

}
