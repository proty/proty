#ifndef PROTY_VM_INSTRUCTIONS_HH
#define PROTY_VM_INSTRUCTIONS_HH

#include "objects.hh"

namespace proty {

  inline Object* add(Object*, Object*);
  inline Object* sub(Object*, Object*);
  inline Object* mul(Object*, Object*);
  inline Object* div(Object*, Object*);
  inline Object* mod(Object*, Object*);

  inline Object* eq(Object*, Object*);
  inline Object* ne(Object*, Object*);
  inline Object* gt(Object*, Object*);
  inline Object* ge(Object*, Object*);
  inline Object* lt(Object*, Object*);
  inline Object* le(Object*, Object*);

  inline Object* getattr(Object*, Object*);
  inline Object* setattr(Object*, Object*, Object*);

  inline Object* newint(int);
  inline Object* newbool(bool);
  inline Object* newstring(char*);
  inline Object* newfunc(FunctionPtr*, unsigned int);
  inline Object* newclass();

  inline Object* call(Object*, int, ...);

  inline bool tobool(Object*);
  inline void print(Object*);

}

#endif