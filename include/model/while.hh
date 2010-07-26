#ifndef PYRITE_MODEL_WHILE_HH
#define PYRITE_MODEL_WHILE_HH

namespace pyrite {

  class WhileModel : public ExprModel {
    private:
      ExprModel* cond;
      BlockModel* block;

    public:
      WhileModel(ExprModel* cond, BlockModel* block)
        : cond(cond), block(block) { }

      Value* codegen(Compiler*);
  };

}

#endif
