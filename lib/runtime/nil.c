#include "runtime.h"

Object* Nil_str(Object* self) {
  return String_new("<nil>");
}

Object* Nil_create() {
  Object* nil = Object_new(Object_proto);

  Object_setSlot(nil, "str", FUNC(Nil_str, 1));

  return nil;
}
