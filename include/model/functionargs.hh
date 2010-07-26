#ifndef PYRITE_FUNCTIONARGS_HH
#define PYRITE_FUNCTIONARGS_HH

#include <vector>

namespace pyrite {

  class FunctionArgsModel {
    private:
      std::vector<std::string> names;
      std::vector<TypeModel*> types;

    public:
      FunctionArgsModel() { }

      void push(std::string, TypeModel*);
      unsigned int size();
      std::string getName(int);
      TypeModel* getType(int);
  };

}

#endif
