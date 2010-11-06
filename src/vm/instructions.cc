#include "objects/objects.hh"
#include <cstdio>

using namespace pyrite;

extern "C" {

  Object* add(Object* a, Object* b) {
    return a->operator+(b);
  }

  Object* sub(Object* a, Object* b) {
    return a->operator-(b);
  }

  Object* mul(Object* a, Object* b) {
    return a->operator*(b);
  }

  Object* div(Object* a, Object* b) {
    return a->operator/(b);
  }

  Object* mod(Object* a, Object* b) {
    return a->operator%(b);
  }

  Object* eq(Object* a, Object* b) {
    return a->operator==(b);
  }

  Object* ne(Object* a, Object* b) {
    return a->operator!=(b);
  }

  Object* gt(Object* a, Object* b) {
    return a->operator>(b);
  }

  Object* ge(Object* a, Object* b) {
    return a->operator>=(b);
  }

  Object* lt(Object* a, Object* b) {
    return a->operator<(b);
  }

  Object* le(Object* a, Object* b) {
    return a->operator<=(b);
  }

  Object* newint(int value) {
    return new Integer(value);
  }

  Object* newbool(bool value) {
    return Bool::get(value);
  }

  Object* newstring(char* value) {
    return new String(value);
  }

  void print(Object* obj) {
    printf("%i\n", (int)*(Integer*)obj);
  }

}