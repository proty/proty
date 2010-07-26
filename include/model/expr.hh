#ifndef PYRITE_MODEL_EXPR_HH
#define PYRITE_MODEL_EXPR_HH

namespace pyrite {

  class ExprModel {
    public:
      virtual Value* codegen(Compiler*) = 0;
  };

}

#endif
