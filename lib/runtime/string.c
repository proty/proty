#include <stdlib.h>
#include <string.h>
#include "runtime.h"

Object* String_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "+", Function_new((FuncPtr)String_add, 2));

  return proto;
}

Object* String_new(const char* value) {
  Object* new = Object_new(String_proto);
  char* v = malloc(sizeof(char)*strlen(value));
  strcpy(v, value);
  new->data.ptr = v;
  return new;
}

Object* String_add(Object* self, Object* other) {
  char* new = strcat(self->data.ptr, other->data.ptr);
  return String_new(new);
}
