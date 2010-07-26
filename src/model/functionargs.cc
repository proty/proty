#include "model/models.hh"

namespace pyrite {

  void FunctionArgsModel::push(std::string name, TypeModel* type) {
    names.push_back(name);
    types.push_back(type);
  }

  unsigned int FunctionArgsModel::size() {
    return names.size();
  }

  std::string FunctionArgsModel::getName(int i) {
    return names.at(i);
  }

  TypeModel* FunctionArgsModel::getType(int i) {
    return types.at(i);
  }

}
