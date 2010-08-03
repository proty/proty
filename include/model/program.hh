#ifndef PYRITE_MODEL_PROGRAM_HH
#define PYRITE_MODEL_PROGRAM_HH

#include <vector>

namespace pyrite {

  class ClassModel;
  class FunctionModel;

  class ProgramModel : public BlockModel {
    private:
      std::vector<ClassModel*> classes;
      std::vector<FunctionModel*> functions;

    public:
      ProgramModel() {}

      void mergeIn(ProgramModel*);

      void addClass(ClassModel*);
      void addFunction(FunctionModel*);

      void generate(Compiler* c);
  };

}

#endif