#ifndef PYRITE_MODEL_CLASS_HH
#define PYRITE_MODEL_CLASS_HH

#include <map>
#include <vector>

namespace pyrite {

  class ClassModel : public ExprModel {
  private:
    std::string name;
    std::vector<std::string> bases;
    std::vector<FunctionModel*> methods;

  public:
    ClassModel(std::string name) : name(name) {}

    void addBase(std::string);

    Value* codegen(Compiler*);
  };

}

#endif
