#ifndef PYRITE_MODEL_INTEGER_HH
#define PYRITE_MODEL_INTEGER_HH

namespace pyrite {

  class IntegerModel : public ExprModel {
    private:
      int value;
      bool primitive;

    public:
      IntegerModel(int value, bool primitive=false)
        : value(value), primitive(primitive) { }

      Value* codegen(Compiler*);
  };

}

#endif
