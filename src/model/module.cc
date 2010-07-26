#include "model/models.hh"

namespace pyrite {

  void ModuleModel::add_function(FunctionModel* fm) {
    functions.push_back(fm);
  }

  void ModuleModel::add_class(ClassModel* cm) {
    classes.push_back(cm);
  }

  void ModuleModel::add_attribute(std::string name, ExprModel* value) {
    attributes[name] = value;
  }

  Value* ModuleModel::codegen(Compiler* c) {
    return 0;
  }

}
