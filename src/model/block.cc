#include "model/models.hh"

namespace pyrite {

  void BlockModel::addExpr(ExprModel* expr) {
    exprs.push_back(expr);
  }

  void BlockModel::codegen(Compiler* c) {
    std::vector<ExprModel*>::iterator it;
    for (it = exprs.begin(); it != exprs.end(); it++) {
      (*it)->codegen(c);
    }
  }

}