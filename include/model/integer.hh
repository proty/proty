#ifndef PYRITE_MODEL_INTEGER_HH
#define PYRITE_MODEL_INTEGER_HH

namespace pyrite {

  class IntegerModel : public ExprModel {
  private:
    int value;

  public:
    IntegerModel(int value)
      : value(value) { }

    Value* codegen(Compiler*);
  };

}

#endif
