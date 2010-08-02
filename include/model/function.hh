#ifndef PYRITE_FUNCTION_HH
#define PYRITE_FUNCTION_HH

namespace pyrite {

  class FunctionModel {
    private:
      std::string name;
      FunctionArgsModel* args;
      TypeModel* returnType;
      BlockModel* block;

    public:
      FunctionModel(std::string name, FunctionArgsModel* args, TypeModel* returnType, BlockModel* block)
        : name(name), args(args), returnType(returnType), block(block) { }

      std::string get_name();

      void generate_prototype(Compiler* c);
      void generate_function(Compiler* c);
  };

}

#endif
