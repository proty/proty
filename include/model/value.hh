#ifndef PYRITE_MODEL_VALUE_HH
#define PYRITE_MODEL_VALUE_HH

namespace pyrite {

  class ValueModel : public ExprModel {
    private:
      Value* value;

    public:
      ValueModel(Value* value)
        : value(value) { }

      Value* codegen(Compiler*) { return value; }
  };

}

#endif
