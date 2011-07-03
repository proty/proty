#include "runtime.h"
#include <stdio.h>

Object* Integer_new(int value) {
  Object* new = Object_new(Integer_proto);
  new->data.i = value;
  return new;
}

Object* Integer_str(Object* self) {
  char buffer[20];
  sprintf(buffer, "%d", self->data.i);
  return String_new(buffer);
}

Object* Integer_bool(Object* self) {
  return self->data.i ? Qtrue : Qfalse;
}

Object* Integer_add(Object* self, Object* other) {
  return Integer_new(self->data.i + other->data.i);
}

Object* Integer_sub(Object* self, Object* other) {
  return Integer_new(self->data.i - other->data.i);
}

Object* Integer_mul(Object* self, Object* other) {
  return Integer_new(self->data.i * other->data.i);
}

Object* Integer_div(Object* self, Object* other) {
  return Integer_new(self->data.i / other->data.i);
}

Object* Integer_eq(Object* self, Object* other) {
  return self->data.i == other->data.i ? Qtrue : Qfalse;
}

Object* Integer_ne(Object* self, Object* other) {
  return self->data.i != other->data.i ? Qtrue : Qfalse;
}

Object* Integer_lt(Object* self, Object* other) {
  return self->data.i < other->data.i ? Qtrue : Qfalse;
}

Object* Integer_gt(Object* self, Object* other) {
  return self->data.i > other->data.i ? Qtrue : Qfalse;
}

Object* Integer_le(Object* self, Object* other) {
  return self->data.i <= other->data.i ? Qtrue : Qfalse;
}

Object* Integer_ge(Object* self, Object* other) {
  return self->data.i >= other->data.i ? Qtrue : Qfalse;
}

Object* Integer_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "str", Function_new((FuncPtr)Integer_str, 1));
  Object_setSlot(proto, "bool", Function_new((FuncPtr)Integer_bool, 1));

  Object_setSlot(proto, "+", Function_new((FuncPtr)Integer_add, 2));
  Object_setSlot(proto, "-", Function_new((FuncPtr)Integer_sub, 2));
  Object_setSlot(proto, "*", Function_new((FuncPtr)Integer_mul, 2));
  Object_setSlot(proto, "/", Function_new((FuncPtr)Integer_div, 2));

  Object_setSlot(proto, "==", Function_new((FuncPtr)Integer_eq, 2));
  Object_setSlot(proto, "!=", Function_new((FuncPtr)Integer_ne, 2));
  Object_setSlot(proto, "<", Function_new((FuncPtr)Integer_lt, 2));
  Object_setSlot(proto, ">", Function_new((FuncPtr)Integer_gt, 2));
  Object_setSlot(proto, "<=", Function_new((FuncPtr)Integer_le, 2));
  Object_setSlot(proto, ">=", Function_new((FuncPtr)Integer_ge, 2));

  return proto;
}
