#include <stdlib.h>
#include <stdarg.h>
#include "runtime.h"

Object* Object_createProto() {
  Object* proto = malloc(sizeof(Object));
  return proto;
}

Object* Object_new(Object* proto) {
  Object* new = malloc(sizeof(Object));
  new->proto = proto;
  return new;
}

Object* Object_setSlot(Object* self, Object* key, Object* value) {
  Hash_set(self->slots, key, value);
  return 0;
}

Object* Object_getSlot(Object* self, Object* key) {
  Hash_get(self->slots, key);
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

unsigned int Object_hash(Object* self) {
  unsigned int hash = 5381;
  int c;

  const char* str = self->data.ptr;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}
