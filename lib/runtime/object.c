#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "runtime.h"

Object* Object_new(Object* proto) {
  Object* new = malloc(sizeof(Object));
  new->proto = proto;
  new->slots = 0;
  return new;
}

Object* Object_setSlot(Object* self, const char* key, Object* value) {
  if (!self->slots) self->slots = Hash_new();
  Hash_set(self->slots, Symbol_get(key), value);
  return value;
}

Object* Object_getSlot(Object* self, const char* key) {
  Object* proto = self;

  while (1) {
    if (proto->slots) {
      Object* value = Hash_get(proto->slots, Symbol_get(key));
      if (value != Qnil) return value;
    }

    if (proto->proto) proto = proto->proto;
    else break;
  }
  return Qnil;
}

Object* Object_call(Object* self, int argc, ...) {
  /// @todo: throw exception
  if (self->proto != Function_proto) {
    printf("proty: called non-function object\n");
    abort();
  }

  va_list varargs;

  Object** argv = malloc(sizeof(Object*)*argc);

  va_start(varargs, argc);
  for (int i = 0; i < argc; i++) {
    argv[i] = va_arg(varargs, Object*);
  }
  va_end(varargs);

  Object* ret = Function_call(self, argc, argv);
  free(argv);

  return ret;
}

Object* Object_bool(Object* self) {
  return (self == Qfalse) ? Qfalse : Qtrue;
}

void Object_initProto() {
  Object_setSlot(Object_proto, "new", FUNC(Object_new, 1));
  Object_setSlot(Object_proto, "bool", FUNC(Object_bool, 1));
}
