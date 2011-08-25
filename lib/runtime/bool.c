#include "runtime.h"

Object* Bool_str(Object* self) {
  if (self == Qtrue) return String_new("<true>");
  else return String_new("<false>");
}

Object* Bool_eq(Object* self, Object* other) {
  if (other == self) return Qtrue;
  Object* otherBoolFunc = Object_getSlot(other, "bool");
  Object* otherBool = Object_call(otherBoolFunc, 1, other);
  return self == otherBool ? Qtrue : Qfalse;
}

Object* Bool_and(Object* self, Object* other) {
  return (self == Qtrue && other == Qtrue) ? Qtrue : Qfalse;
}

Object* Bool_or(Object* self, Object* other) {
  return (self == Qtrue || other == Qtrue) ? Qtrue : Qfalse;
}

Object* Bool_not(Object* self) {
  return self == Qtrue ? Qfalse : Qtrue;
}

Object* Bool_new() {
  return Object_new(Bool_proto);
}

void Bool_initProto() {
  Object_setSlot(Bool_proto, "==", FUNC(Bool_eq, 2));
  Object_setSlot(Bool_proto, "&&", FUNC(Bool_and, 2));
  Object_setSlot(Bool_proto, "||", FUNC(Bool_or, 2));
  Object_setSlot(Bool_proto, "!", FUNC(Bool_not, 1));
  Object_setSlot(Bool_proto, "str", FUNC(Bool_str, 1));
}
