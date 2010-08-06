#ifndef PYRITE_MODEL_CLASS_HH
#define PYRITE_MODEL_CLASS_HH

namespace pyrite {

  class ClassModel {
    private:
      std::string name;
      std::vector<std::string> bases;
      std::vector<FunctionModel*> methods;
      std::map<std::string, TypeModel*> attributes;
      PATypeHolder pholder;

    public:
      ClassModel(std::string name) : name(name) {}

      std::string getName();
      const Type* getType(Compiler*);

      void addBase(std::string);
      void addMethod(FunctionModel*);
      void addAttribute(std::string, TypeModel*);

      void generatePlaceholder(Compiler*);
      void generateType(Compiler*);
      void generateMethodPrototypes(Compiler*);
      void generateMethodFunctions(Compiler*);
  };

}

#endif
