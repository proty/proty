#include <stdlib.h>
#include <string.h>
#include "runtime.h"

Object* String_new(const char* value) {
  Object* new = Object_new(String_proto);
  new->data.ptr = malloc(strlen(value)+1);
  strcpy(new->data.ptr, value);
  return new;
}

Object* String_add(Object* self, Object* other) {
  int size = strlen(self->data.ptr)+strlen(other->data.ptr)+1;
  char* buffer = malloc(size);
  strcpy(buffer, self->data.ptr);
  strcat(buffer, other->data.ptr);
  Object* str = String_new(buffer);
  free(buffer);
  return str;
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

Object* String_length(Object* self) {
  return Integer_new(strlen(self->data.ptr));
}

void String_initProto() {
  Object_setSlot(String_proto, "+", FUNC(String_add, 2));

  Object_setSlot(String_proto, "==", FUNC(String_eq, 2));
  Object_setSlot(String_proto, "!=", FUNC(String_ne, 2));

  Object_setSlot(String_proto, "bool", FUNC(String_bool, 1));

  Object_setSlot(String_proto, "length", FUNC(String_length, 1));
}

