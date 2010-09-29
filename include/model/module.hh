#ifndef PYRITE_MODEL_MODULE_HH
#define PYRITE_MODEL_MODULE_HH

namespace pyrite {

  class ModuleModel {
  private:
    std::string name;
    std::vector<FunctionModel*> functions;
    std::vector<ClassModel*> classes;
    std::map<std::string, ExprModel*> attributes;

  public:
    ModuleModel(std::string name)
      : name(name) { }

    void addFunction(FunctionModel*);
    void addClass(ClassModel*);
    void addAttribute(std::string, ExprModel*);
  };

}

#endif
