#ifndef PYRITE_MODEL_CALL_HH
#define PYRITE_MODEL_CALL_HH

namespace pyrite {

  class CallModel : public ExprModel {
    private:
      NameModel* name;
      CallArgsModel* args;

    public:
      CallModel(NameModel* name, CallArgsModel* args)
        : name(name), args(args) { }

      Value* codegen(Compiler*);
  };

}

#endif
