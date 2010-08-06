#ifndef PYRITE_MODEL_NIL_HH
#define PYRITE_MODEL_NIL_HH

namespace pyrite {

  class NilModel : public ExprModel {
    public:
      NilModel() { }

      Value* codegen(Compiler*);
  };

}

#endif