#ifndef PYRITE_MODEL_PROGRAM_HH
#define PYRITE_MODEL_PROGRAM_HH

#include <vector>

namespace pyrite {

  class ClassModel;
  class FunctionModel;

  class ProgramModel : public BlockModel {
  protected:
    std::vector<std::string> imports;
    std::vector<ClassModel*> classes;
    std::vector<FunctionModel*> functions;

  public:
    ProgramModel() {}

    void mergeIn(ProgramModel*, bool=false);

    void addImport(std::string);
    void addClass(ClassModel*);
    void addFunction(FunctionModel*);

    void generate(Compiler* c);
  };

}

#endif