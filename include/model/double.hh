#ifndef PYRITE_MODEL_DOUBLE_HH
#define PYRITE_MODEL_DOUBLE_HH

namespace pyrite {

  class DoubleModel : public ExprModel {
    private:
      double value;
      bool primitive;

    public:
      DoubleModel(double value, bool primitive=false)
        : value(value), primitive(primitive) { }

      Value* codegen(Compiler*);
  };

}

#endif
