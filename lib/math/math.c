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
  MODINIT(math_sin, FUNC(math_sin, 2));
  MODINIT(math_cos, FUNC(math_cos, 2));
  MODINIT(math_tan, FUNC(math_cos, 2));
  MODINIT(math_asin, FUNC(math_asin, 2));
  MODINIT(math_acos, FUNC(math_acos, 2));
  MODINIT(math_atan, FUNC(math_atan, 2));

  MODINIT(math_log, FUNC(math_log, 2));

  MODINIT(math_pow, FUNC(math_pow, 3));
  MODINIT(math_sqrt, FUNC(math_sqrt, 2));

  MODINIT(math_pi, Float_new(3.141592));
  MODINIT(math_e, Float_new(2.718281));
}
