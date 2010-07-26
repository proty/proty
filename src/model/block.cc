#include "model/models.hh"

namespace pyrite {

  void BlockModel::push(ExprModel* expr) {
    exprs.push_back(expr);
  }

  unsigned int BlockModel::size() {
    return exprs.size();
  }

  ExprModel* BlockModel::get(int pos) {
    return exprs.at(pos);
  }

}
