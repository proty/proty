#include "model/models.hh"

namespace pyrite {

  void ProgramModel::mergeIn(ProgramModel* p) {
    classes.insert(classes.end(), p->classes.begin(), p->classes.end());
    functions.insert(functions.end(), p->functions.begin(), p->functions.end());
  }

  void ProgramModel::addClass(ClassModel* c) {
    classes.push_back(c);
  }

  void ProgramModel::addFunction(FunctionModel* f) {
    functions.push_back(f);
  }

  void ProgramModel::generate(Compiler* c) {
    std::vector<ClassModel*>::iterator cit;
    std::vector<FunctionModel*>::iterator fit;

    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generatePlaceholder(c);

    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generateType(c);

    for (fit = functions.begin(); fit != functions.end(); ++fit)
      (*fit)->generatePrototype(c);

    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generateMethodPrototypes(c);

    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generateMethodFunctions(c);

    for (fit = functions.begin(); fit != functions.end(); ++fit)
      (*fit)->generateFunction(c);

    codegen(c);
  }

}