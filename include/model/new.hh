#ifndef PYRITE_NEW_HH
#define PYRITE_NEW_HH

namespace pyrite {

  class NewModel : public ExprModel {
    private:
      std::string name;
      CallArgsModel* args;

    public:
      NewModel(std::string name, CallArgsModel* args)
        : name(name), args(args) { }

      Value* codegen(Compiler*);
  };

}

#endif
