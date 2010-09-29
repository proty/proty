#ifndef PYRITE_MODEL_IF_HH
#define PYRITE_MODEL_IF_HH

namespace pyrite {

  class IfModel : public ExprModel {
  private:
    ExprModel* cond;
    BlockModel* thenBlock;
    BlockModel* elseBlock;

  public:
    IfModel(ExprModel* cond, BlockModel* thenBlock, BlockModel* elseBlock)
      : cond(cond), thenBlock(thenBlock), elseBlock(elseBlock) { }

    Value* codegen(Compiler*);
  };

}

#endif
