#ifndef PYRITE_VM_INSTRUCTIONS_HH
#define PYRITE_VM_INSTRUCTIONS_HH

#include "objects.hh"

namespace pyrite {

  Object* add(Object*, Object*);
  Object* sub(Object*, Object*);
  Object* mul(Object*, Object*);
  Object* div(Object*, Object*);
  Object* mod(Object*, Object*);

  Object* eq(Object*, Object*);
  Object* ne(Object*, Object*);
  Object* gt(Object*, Object*);
  Object* ge(Object*, Object*);
  Object* lt(Object*, Object*);
  Object* le(Object*, Object*);

  Object* getattr(Object*, Object*);
  Object* setattr(Object*, Object*, Object*);

  Object* newint(int);
  Object* newbool(bool);
  Object* newstring(char*);
  Object* newfunc(FunctionPtr*, unsigned int);
  Object* newclass();

  Object* call(Object*, int, ...);

  bool tobool(Object*);
  void print(Object*);

}

#endif