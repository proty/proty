#include "runtime.h"

Object* Bool_create() {
  Object* bool = Object_new(Object_proto);

  Object_setSlot(bool, "str", Function_new((FuncPtr)Bool_str));

  return bool;
}

Object* Bool_str(Object* self) {
  if (self == Qtrue) return String_new("<true>");
  else return String_new("<false>");
}
