#include "model/models.hh"

namespace pyrite {

  void FunctionArgsModel::push(std::string name) {
    names.push_back(name);
  }

  unsigned int FunctionArgsModel::size() {
    return names.size();
  }

  std::string FunctionArgsModel::get(int i) {
    return names.at(i);
  }

}
