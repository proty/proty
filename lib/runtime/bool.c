#include "runtime.h"

Object* Bool_str(Object* self) {
  if (self == Qtrue) return String_new("<true>");
  else return String_new("<false>");
}

Object* Bool_create() {
  Object* boolean = Object_new(Object_proto);

  Object_setSlot(boolean, "str", FUNC(Bool_str, 1));

  return boolean;
}
