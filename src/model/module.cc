#include "model/models.hh"

namespace pyrite {

  void ModuleModel::addFunction(FunctionModel* fm) {
    functions.push_back(fm);
  }

  void ModuleModel::addClass(ClassModel* cm) {
    classes.push_back(cm);
  }

  void ModuleModel::addAttribute(std::string name, ExprModel* value) {
    attributes[name] = value;
  }

}
