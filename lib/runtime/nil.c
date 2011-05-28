#include "runtime.h"

Object* Nil_create() {
  Object* nil = Object_new(Object_proto);

  Object_setSlot(nil, "str", Function_new((FuncPtr)Nil_str));

  return nil;
}

Object* Nil_str() {
  return String_new("<nil>");
}
