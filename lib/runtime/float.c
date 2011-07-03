#include "runtime.h"
#include <stdio.h>

Object* Float_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "str", Function_new((FuncPtr)Float_str, 1));
  Object_setSlot(proto, "bool", Function_new((FuncPtr)Float_bool, 1));

  Object_setSlot(proto, "+", Function_new((FuncPtr)Float_add, 2));
  Object_setSlot(proto, "-", Function_new((FuncPtr)Float_sub, 2));
  Object_setSlot(proto, "*", Function_new((FuncPtr)Float_mul, 2));
  Object_setSlot(proto, "/", Function_new((FuncPtr)Float_div, 2));

  Object_setSlot(proto, "==", Function_new((FuncPtr)Float_eq, 2));
  Object_setSlot(proto, "!=", Function_new((FuncPtr)Float_ne, 2));
  Object_setSlot(proto, "<", Function_new((FuncPtr)Float_lt, 2));
  Object_setSlot(proto, ">", Function_new((FuncPtr)Float_gt, 2));
  Object_setSlot(proto, "<=", Function_new((FuncPtr)Float_le, 2));
  Object_setSlot(proto, ">=", Function_new((FuncPtr)Float_ge, 2));

  return proto;
}

Object* Float_new(double value) {
  Object* new = Object_new(Float_proto);
  new->data.d = value;
  return new;
}

Object* Float_str(Object* self) {
  char buffer[20];
  sprintf(buffer, "%f", self->data.d);
  return String_new(buffer);
}

Object* Float_bool(Object* self) {
  return self->data.d ? Qtrue : Qfalse;
}

Object* Float_add(Object* self, Object* other) {
  return Float_new(self->data.d + other->data.d);
}

Object* Float_sub(Object* self, Object* other) {
  return Float_new(self->data.d - other->data.d);
}

Object* Float_mul(Object* self, Object* other) {
  return Float_new(self->data.d * other->data.d);
}

Object* Float_div(Object* self, Object* other) {
  return Float_new(self->data.d / other->data.d);
}

Object* Float_eq(Object* self, Object* other) {
  return self->data.d == other->data.d ? Qtrue : Qfalse;
}

Object* Float_ne(Object* self, Object* other) {
  return self->data.d != other->data.d ? Qtrue : Qfalse;
}

Object* Float_lt(Object* self, Object* other) {
  return self->data.d < other->data.d ? Qtrue : Qfalse;
}

Object* Float_gt(Object* self, Object* other) {
  return self->data.d > other->data.d ? Qtrue : Qfalse;
}

Object* Float_le(Object* self, Object* other) {
  return self->data.d <= other->data.d ? Qtrue : Qfalse;
}

Object* Float_ge(Object* self, Object* other) {
  return self->data.d >= other->data.d ? Qtrue : Qfalse;
}
