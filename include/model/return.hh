#ifndef PYRITE_MODEL_RETURN_HH
#define PYRITE_MODEL_RETURN_HH

namespace pyrite {

  class ReturnModel : public ExprModel {
    private:
      ExprModel* value;

    public:
      ReturnModel(ExprModel* value)
        : value(value) { }

      Value* codegen(Compiler*);
  };

}

#endif
