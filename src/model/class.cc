#include "model/models.hh"

namespace pyrite {

  void ClassModel::add_method(FunctionModel* function) {
    methods.push_back(function);
  }

  Value* ClassModel::codegen(Compiler* c) {
    name = c->env->format_class_name(name);
    c->env->set_class(name);

    std::vector<const Type*> elts;
    StructType* classtype = StructType::get(getGlobalContext(), elts);
    c->module->addTypeName(name, classtype);

    for (unsigned int i = 0; i < methods.size(); i++) {
      methods.at(i)->codegen(c);
    }

    c->env->clear_class();
    return 0;
  }

}
