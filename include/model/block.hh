#ifndef PYRITE_MODEL_BLOCK_HH
#define PYRITE_MODEL_BLOCK_HH

#include <vector>

namespace pyrite {

  class BlockModel {
  protected:
    std::vector<ExprModel*> exprs;

  public:
    BlockModel() { }

    void addExpr(ExprModel*);
    void codegen(Compiler*);
  };

}

#endif