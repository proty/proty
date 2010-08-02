#ifndef PYRITE_MODEL_CLASS_HH
#define PYRITE_MODEL_CLASS_HH

namespace pyrite {

  class ClassModel {
    private:
      std::string name;
      std::vector<FunctionModel*> methods;
      std::map<std::string, TypeModel*> attributes;
      PATypeHolder pholder;

    public:
      ClassModel(std::string name) : name(name) {}

      void add_method(FunctionModel*);
      void add_attribute(std::string, TypeModel*);

      void generate_placeholder(Compiler*);
      void generate_type(Compiler*);
      void generate_methods(Compiler*);

      std::string get_name();
  };

}

#endif
