#include "runtime/runtime.h"
#include <math.h>

MODFUNC(math_sin, Object* self, Object* num) {
  return Float_new(sin(num->data.d));
}

MODFUNC(math_cos, Object* self, Object* num) {
  return Float_new(cos(num->data.d));
}

MODFUNC(math_tan, Object* self, Object* num) {
  return Float_new(tan(num->data.d));
}

MODFUNC(math_asin, Object* self, Object* num) {
  return Float_new(asin(num->data.d));
}

MODFUNC(math_acos, Object* self, Object* num) {
  return Float_new(acos(num->data.d));
}

MODFUNC(math_atan, Object* self, Object* num) {
  return Float_new(atan(num->data.d));
}

MODFUNC(math_log, Object* self, Object* num) {
  return Float_new(log(num->data.d));
}

MODFUNC(math_pow, Object* self, Object* base, Object* exponent) {
  return Float_new(pow(base->data.d, exponent->data.d));
}

MODFUNC(math_sqrt, Object* self, Object* num) {
  return Float_new(sqrt(num->data.d));
}

EXPORT(math_pi);
EXPORT(math_e);

void math_init() {
  INITFUNC(math_sin, 2);
  INITFUNC(math_cos, 2);
  INITFUNC(math_tan, 2);
  INITFUNC(math_asin, 2);
  INITFUNC(math_acos, 2);
  INITFUNC(math_atan, 2);

  INITFUNC(math_log, 2);

  INITFUNC(math_pow, 3);
  INITFUNC(math_sqrt, 2);

  INITOBJ(math_pi, Float_new(3.141592));
  INITOBJ(math_e, Float_new(2.718281));
}
