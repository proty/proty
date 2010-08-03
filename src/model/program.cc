#include "model/models.hh"

#include <iostream>

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

    std::cout << "generate class placeholders" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generatePlaceholder(c);

    std::cout << "generate class types" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generateType(c);

    std::cout << "generate function prototypes" << std::endl;
    for (fit = functions.begin(); fit != functions.end(); ++fit)
      (*fit)->generatePrototype(c);

    std::cout << "generate class method prototypes" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generateMethodPrototypes(c);

    std::cout << "generate class method functions" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generateMethodFunctions(c);

    std::cout << "generate functions" << std::endl;
    for (fit = functions.begin(); fit != functions.end(); ++fit)
      (*fit)->generateFunction(c);

    std::cout << "generate main code" << std::endl;
    codegen(c);
  }

}