#ifndef PYRITE_MODEL_METHODCALL_HH
#define PYRITE_MODEL_METHODCALL_HH

namespace pyrite {

  class MethodCallModel : public ExprModel {
    private:
      ExprModel* instance;
      std::string name;
      CallArgsModel* args;

    public:
      MethodCallModel(ExprModel* instance, std::string name, CallArgsModel* args=0)
        : instance(instance), name(name), args(args) { }

      Value* codegen(Compiler*);
  };

}

#endif
