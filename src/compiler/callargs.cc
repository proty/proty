#include "compiler/models.hh"

namespace pyrite {
  
  void CallArgsModel::push(ExprModel* e) {
    values.push_back(e);
  }
  
  unsigned int CallArgsModel::size() {
    return values.size();
  }
  
  ExprModel* CallArgsModel::get(int i) {
    return values.at(i);
  }

}