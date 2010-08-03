#ifndef PYRITE_FUNCTION_HH
#define PYRITE_FUNCTION_HH

namespace pyrite {

  class FunctionModel {
    private:
      std::string name;
      FunctionArgsModel* args;
      TypeModel* returnType;
      BlockModel* block;
      ClassModel* klass;

    public:
      FunctionModel(std::string name, FunctionArgsModel* args, TypeModel* returnType, BlockModel* block)
        : name(name), args(args), returnType(returnType), block(block) { }

      std::string get_name();
      void methodOf(ClassModel*);

      void generatePrototype(Compiler* c);
      void generateFunction(Compiler* c);
  };

}

#endif
