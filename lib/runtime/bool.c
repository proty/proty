#include "runtime.h"

Object* Bool_str(Object* self) {
  if (self == Qtrue) return String_new("<true>");
  else return String_new("<false>");
}

Object* Bool_new() {
  return Object_new(Bool_proto);
}

void Bool_initProto() {
  Object_setSlot(Bool_proto, "str", FUNC(Bool_str, 1));
}
