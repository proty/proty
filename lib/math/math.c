#include "runtime/runtime.h"
#include <math.h>

Object* math_sin(Object* self, Object* num) {
  return Float_new(sin(num->data.d));
}

Object* math_cos(Object* self, Object* num) {
  return Float_new(cos(num->data.d));
}

Object* math_tan(Object* self, Object* num) {
  return Float_new(tan(num->data.d));
}

Object* math_asin(Object* self, Object* num) {
  return Float_new(asin(num->data.d));
}

Object* math_acos(Object* self, Object* num) {
  return Float_new(acos(num->data.d));
}

Object* math_atan(Object* self, Object* num) {
  return Float_new(atan(num->data.d));
}

Object* math_log(Object* self, Object* num) {
  return Float_new(log(num->data.d));
}

Object* math_pow(Object* self, Object* base, Object* exponent) {
  return Float_new(pow(base->data.d, exponent->data.d));
}

Object* math_sqrt(Object* self, Object* num) {
  return Float_new(sqrt(num->data.d));
}

Object* math_init() {
  Object* math = Object_new(Object_proto);

  Object_setSlot(math, "sin", Function_new((FuncPtr)math_sin, 2));
  Object_setSlot(math, "cos", Function_new((FuncPtr)math_cos, 2));
  Object_setSlot(math, "tan", Function_new((FuncPtr)math_cos, 2));
  Object_setSlot(math, "asin", Function_new((FuncPtr)math_asin, 2));
  Object_setSlot(math, "acos", Function_new((FuncPtr)math_acos, 2));
  Object_setSlot(math, "atan", Function_new((FuncPtr)math_atan, 2));

  Object_setSlot(math, "log", Function_new((FuncPtr)math_log, 2));

  Object_setSlot(math, "pow", Function_new((FuncPtr)math_pow, 3));
  Object_setSlot(math, "sqrt", Function_new((FuncPtr)math_sqrt, 2));

  Object_setSlot(math, "pi", Float_new(3.141592));
  Object_setSlot(math, "e", Float_new(2.718281));

  return math;
}
