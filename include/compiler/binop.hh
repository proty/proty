#ifndef PYRITE_MODEL_BINOP_HH
#define PYRITE_MODEL_BINOP_HH

namespace pyrite {

  class BinOpModel : public ExprModel {
  private:
    ExprModel* lhs;
    ExprModel* rhs;
    std::string op;

  public:
    BinOpModel(ExprModel* lhs, ExprModel* rhs, std::string op)
      : lhs(lhs), rhs(rhs), op(op) { }

    Value* codegen(Compiler*);
  };

}

#endif
