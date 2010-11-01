#ifndef PYRITE_MODEL_FUNCTION_HH
#define PYRITE_MODEL_FUNCTION_HH

namespace pyrite {

  class FunctionModel : public ExprModel {
  private:
    std::string name;
    FunctionArgsModel* args;
    BlockModel* block;

  public:
    FunctionModel(std::string name, FunctionArgsModel* args, BlockModel* block)
      : name(name), args(args), block(block) {}

    Value* codegen(Compiler*);
  };

}

#endif
