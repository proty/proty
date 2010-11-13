#ifndef PYRITE_MODEL_CALL_HH
#define PYRITE_MODEL_CALL_HH

namespace pyrite {

  class CallModel : public ExprModel {
  private:
    std::string name;
    CallArgsModel* args;

  public:
    CallModel(std::string name, CallArgsModel* args=0)
      : name(name), args(args) { }

    Value* codegen(Compiler*);
  };

}

#endif
