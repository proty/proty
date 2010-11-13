#ifndef PYRITE_MODEL_CLASS_HH
#define PYRITE_MODEL_CLASS_HH

#include <map>
#include <vector>

namespace pyrite {

  class ClassModel : public ExprModel {
  private:
    std::string name;
    std::string base;
    std::vector<FunctionModel*> methods;
    std::map<std::string, ExprModel*> attributes;

  public:
    ClassModel(std::string name) : name(name) {}

    void setBase(std::string);
    void addMethod(FunctionModel*);
    void addAttribute(std::string, ExprModel*);

    std::string getName();

    Value* codegen(Compiler*);
  };

}

#endif
