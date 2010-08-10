#include "model/models.hh"

namespace pyrite {

  void ProgramModel::mergeIn(ProgramModel* p, bool main) {
    imports.insert(imports.end(), p->imports.begin(), p->imports.end());
    classes.insert(classes.end(), p->classes.begin(), p->classes.end());
    functions.insert(functions.end(), p->functions.begin(), p->functions.end());
    if (main) exprs.insert(exprs.end(), p->exprs.begin(), p->exprs.end());
  }

  void ProgramModel::addImport(std::string name) {
    imports.push_back(name);
  }

  void ProgramModel::addClass(ClassModel* c) {
    classes.push_back(c);
  }

  void ProgramModel::addFunction(FunctionModel* f) {
    functions.push_back(f);
  }

  void ProgramModel::generate(Compiler* c) {
    std::vector<std::string>::iterator importIt;
    std::vector<ClassModel*>::iterator classIt;
    std::vector<FunctionModel*>::iterator funcIt;

    // !! imports.end() can be updated during the loop
    for (importIt = imports.begin(); importIt != imports.end(); importIt++)
      c->linkPyrite(*importIt + ".pr");

    for (classIt = classes.begin(); classIt != classes.end(); classIt++)
      (*classIt)->generatePlaceholder(c);

    for (classIt = classes.begin(); classIt != classes.end(); classIt++)
      (*classIt)->generateType(c);

    for (funcIt = functions.begin(); funcIt != functions.end(); funcIt++)
      (*funcIt)->generatePrototype(c);

    for (classIt = classes.begin(); classIt != classes.end(); classIt++)
      (*classIt)->generateMethodPrototypes(c);

    for (classIt = classes.begin(); classIt != classes.end(); classIt++)
      (*classIt)->generateMethodFunctions(c);

    for (funcIt = functions.begin(); funcIt != functions.end(); funcIt++)
      (*funcIt)->generateFunction(c);

    codegen(c);
  }

}