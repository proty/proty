#ifndef PYRITE_MODEL_ATTRIBUTE_HH
#define PYRITE_MODEL_ATTRIBUTE_HH

namespace pyrite {

  class AttributeModel : public ExprModel {
  private:
    ExprModel* instance;
    std::string name;
    ExprModel* value;

  public:
    AttributeModel(ExprModel* instance, std::string name, ExprModel* value=0)
      : instance(instance), name(name), value(value) { }

    Value* codegen(Compiler*);
  };

}

#endif
