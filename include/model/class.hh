#ifndef PYRITE_MODEL_CLASS_HH
#define PYRITE_MODEL_CLASS_HH

namespace pyrite {

  class ClassModel : public ExprModel {
    private:
      std::string name;
      std::vector<FunctionModel*> methods;

    public:
      ClassModel(std::string name)
        : name(name) { }

      void add_method(FunctionModel*);
      std::string get_name();

      Value* codegen(Compiler*);
  };

}

#endif
