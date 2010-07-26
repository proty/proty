#ifndef PYRITE_MODEL_MODULE_HH
#define PYRITE_MODEL_MODULE_HH

namespace pyrite {

  class ModuleModel : public ExprModel {
    private:
      std::string name;
      std::vector<FunctionModel*> functions;
      std::vector<ClassModel*> classes;
      std::map<std::string, ExprModel*> attributes;

    public:
      ModuleModel(std::string name)
        : name(name) { }

      void add_function(FunctionModel*);
      void add_class(ClassModel*);
      void add_attribute(std::string, ExprModel*);

      Value* codegen(Compiler*);
  };

}

#endif
