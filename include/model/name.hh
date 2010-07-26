#ifndef PYRITE_MODEL_NAME_HH
#define PYRITE_MODEL_NAME_HH

namespace pyrite {

  class NameModel : public ExprModel {
    private:
      std::string name;

    public:
      NameModel(std::string name)
        : name(name) { }

      std::string get_name();

      Value* codegen(Compiler*);
  };

}

#endif
