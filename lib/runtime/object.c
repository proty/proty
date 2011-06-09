#include <stdlib.h>
#include <stdarg.h>
#include "runtime.h"

Object* Object_createProto() {
  Object* proto = Object_new(0);

  Object_setSlot(proto, "bool", Function_new((FuncPtr)Object_bool));

  return proto;
}

Object* Object_new(Object* proto) {
  Object* new = malloc(sizeof(Object));
  new->proto = proto;
  new->slots = 0;

  return new;
}

Object* Object_setSlot(Object* self, const char* key, Object* value) {
  if (!self->slots) self->slots = Hash_new();
  Hash_set(self->slots, key, value);
  return value;
}

Object* Object_getSlot(Object* self, const char* key) {
  Object* proto = self;

  while (1) {
    if (proto->slots) {
      Object* value = Hash_get(proto->slots, key);
      if (value) return value;
    }

    if (proto->proto) proto = proto->proto;
    else break;
  }
  return 0;
}

Object* Object_call(Object* self, int argc, ...) {
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
  return Qtrue;
}
