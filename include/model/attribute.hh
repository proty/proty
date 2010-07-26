#ifndef PYRITE_MODEL_ATTRIBUTE_HH
#define PYRITE_MODEL_ATTRIBUTE_HH

namespace pyrite {

  class AttributeModel : public ExprModel {
    private:
      ExprModel* instance;
      std::string name;

    public:
      AttributeModel(ExprModel* instance, std::string name)
        : instance(instance), name(name) { }

      Value* codegen(Compiler*);
  };

}

#endif
