#ifndef PYRITE_MODEL_CALLARGS_HH
#define PYRITE_MODEL_CALLARGS_HH

#include <vector>

namespace pyrite {

  class CallArgsModel {
    private:
      std::vector<ExprModel*> values;

    public:
      CallArgsModel() { }

      void push(ExprModel*);
      unsigned int size();
      ExprModel* get(int);
  };

}

#endif