#ifndef PYRITE_MODEL_ASSIGN_HH
#define PYRITE_MODEL_ASSIGN_HH

namespace pyrite {

  class AssignModel : public ExprModel {
  private:
    std::string name;
    ExprModel* value;
    bool declare;

  public:
    AssignModel(std::string name, ExprModel* value)
      : name(name), value(value) { }

    Value* codegen(Compiler*);
  };

}

#endif
