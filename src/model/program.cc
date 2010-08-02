#include "model/models.hh"

#include <iostream>

namespace pyrite {

  void ProgramModel::merge_in(ProgramModel* p) {
    classes.insert(classes.end(), p->classes.begin(), p->classes.end());
    functions.insert(functions.end(), p->functions.begin(), p->functions.end());
  }

  void ProgramModel::add_class(ClassModel* c) {
    classes.push_back(c);
  }

  void ProgramModel::add_function(FunctionModel* f) {
    functions.push_back(f);
  }

  void ProgramModel::generate(Compiler* c) {
    std::vector<ClassModel*>::iterator cit;
    std::vector<FunctionModel*>::iterator fit;

    std::cout << "generate class placeholders" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generate_placeholder(c);

    std::cout << "generate class types" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generate_type(c);

    std::cout << "generate function prototypes" << std::endl;
    for (fit = functions.begin(); fit != functions.end(); ++fit)
      (*fit)->generate_prototype(c);

    std::cout << "generate class methods" << std::endl;
    for (cit = classes.begin(); cit != classes.end(); ++cit)
      (*cit)->generate_methods(c);

    std::cout << "generate functions" << std::endl;
    for (fit = functions.begin(); fit != functions.end(); ++fit)
      (*fit)->generate_function(c);

    std::cout << "generate main code" << std::endl;
    codegen(c);
  }

}