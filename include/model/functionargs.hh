#ifndef PYRITE_MODEL_FUNCTIONARGS_HH
#define PYRITE_MODEL_FUNCTIONARGS_HH

#include <vector>

namespace pyrite {

  class FunctionArgsModel {
  private:
    std::vector<std::string> names;

  public:
    FunctionArgsModel() { }

    void push(std::string);

    unsigned int size();
    std::string getName(int);
  };

}

#endif
