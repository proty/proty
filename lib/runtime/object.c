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

Object* Object_setSlot(Object* self, Object* key, Object* value) {
  if (!self->slots) self->slots = Hash_new();
  return Hash_set(self->slots, key, value);
}

Object* Object_getSlot(Object* self, Object* key) {
  Object* proto = self;

  while (1) {
    if (proto->slots) {
      Object* value = Hash_get(proto->slots, key);
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
  Object** argv = alloca(sizeof(Object*)*argc);

  // self is nil for calls
  argc++;
  argv[0] = Qnil;

  va_start(varargs, argc);
  for (int i = 1; i < argc; i++) {
    argv[i] = va_arg(varargs, Object*);
  }
  va_end(varargs);

  return Function_call(self, argc, argv);
}

Object* Object_send(Object* self, Object* key, int argc, ...) {
  Object* func = Object_getSlot(self, key);

  if (func->proto != Function_proto) {
    printf("proty: called non-function object\n");
    abort();
  }

  va_list varargs;
  Object** argv = alloca(sizeof(Object*)*argc);
  argv[0] = self;

  va_start(varargs, argc);
  argc++;
  for (int i = 1; i < argc; i++) {
    argv[i] = va_arg(varargs, Object*);
  }
  va_end(varargs);

  return Function_call(func, argc, argv);
}

Object* Object_bool(Object* self) {
  return (self == Qfalse) ? Qfalse : Qtrue;
}

Object* Object_not(Object* self) {
  Object* bool = Object_send(self, SYM(bool), 0);
  return bool == Qtrue ? Qfalse : Qtrue;
}

void Object_initProto() {
  Object_setSlot(Object_proto, SYM(new), FUNC(Object_new, 1));
  Object_setSlot(Object_proto, SYM(bool), FUNC(Object_bool, 1));
  Object_setSlot(Object_proto, SYM(!), FUNC(Object_not, 1));
}
