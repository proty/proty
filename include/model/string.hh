#ifndef PYRITE_MODEL_STRING_HH
#define PYRITE_MODEL_STRING_HH

namespace pyrite {

  class StringModel : public ExprModel {
    private:
      std::string value;
      bool primitive;

    public:
      StringModel(std::string value, bool primitive=false)
        : value(value), primitive(primitive) { }

      Value* codegen(Compiler*);
  };

}

#endif
