#include <stdlib.h>
#include <string.h>
#include "runtime.h"

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

Object* String_eq(Object* self, Object* other) {
  return strcmp(self->data.ptr, other->data.ptr) ? Qfalse : Qtrue;
}

Object* String_ne(Object* self, Object* other) {
  return strcmp(self->data.ptr, other->data.ptr) ? Qtrue : Qfalse;
}

Object* String_bool(Object* self) {
  return strlen(self->data.ptr) ? Qtrue : Qfalse;
}

Object* String_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "+", Function_new((FuncPtr)String_add, 2));

  Object_setSlot(proto, "==", Function_new((FuncPtr)String_eq, 2));
  Object_setSlot(proto, "!=", Function_new((FuncPtr)String_ne, 2));

  Object_setSlot(proto, "bool", Function_new((FuncPtr)String_bool, 1));

  return proto;
}

