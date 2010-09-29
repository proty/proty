#ifndef PYRITE_FUNCTIONARGS_HH
#define PYRITE_FUNCTIONARGS_HH

#include <vector>

namespace pyrite {

  class FunctionArgsModel {
  private:
    std::vector<std::string> names;
    std::vector<TypeModel*> types;
    bool isVarArg;

  public:
    FunctionArgsModel() { isVarArg = false; }

    void push(std::string, TypeModel*);

    void setVarArg();
    bool getVarArg();

    unsigned int size();
    std::string getName(int);
    TypeModel* getType(int);
  };

}

#endif
