#ifndef PYRITE_MODEL_STRING_HH
#define PYRITE_MODEL_STRING_HH

namespace pyrite {

  class StringModel : public ExprModel {
  private:
    std::string value;

  public:
    StringModel(std::string value)
      : value(value) { }

    Value* codegen(Compiler*);
  };

}

#endif
