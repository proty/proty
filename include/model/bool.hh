#ifndef PYRITE_MODEL_BOOL_HH
#define PYRITE_MODEL_BOOL_HH

namespace pyrite {

  class BoolModel : public ExprModel {
    private:
      bool value;

    public:
      BoolModel(bool value)
        : value(value) { }

      Value* codegen(Compiler*);
  };

}

#endif
