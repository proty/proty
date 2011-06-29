#include "runtime.h"

Object* Bool_create() {
  Object* boolean = Object_new(Object_proto);

  Object_setSlot(boolean, "str", Function_new((FuncPtr)Bool_str, 1));

  return boolean;
}

Object* Bool_str(Object* self) {
  if (self == Qtrue) return String_new("<true>");
  else return String_new("<false>");
}
