#ifndef PYRITE_MODEL_DOUBLE_HH
#define PYRITE_MODEL_DOUBLE_HH

namespace pyrite {

  class DoubleModel : public ExprModel {
    private:
      double value;
    
    public:
      DoubleModel(double value)
        : value(value) { }
    
      Value* codegen(Compiler*);
  };

}

#endif
