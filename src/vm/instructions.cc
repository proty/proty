#include "objects/objects.hh"
#include <cstdio>
#include <cstdarg>

using namespace pyrite;

extern "C" {

  Object* add(Object* a, Object* b) {
    return *a + b;
  }

  Object* sub(Object* a, Object* b) {
    return *a - b;
  }

  Object* mul(Object* a, Object* b) {
    return *a * b;
  }

  Object* div(Object* a, Object* b) {
    return *a / b;
  }

  Object* mod(Object* a, Object* b) {
    return *a % b;
  }

  Object* eq(Object* a, Object* b) {
    return *a == b;
  }

  Object* ne(Object* a, Object* b) {
    return *a != b;
  }

  Object* gt(Object* a, Object* b) {
    return *a > b;
  }

  Object* ge(Object* a, Object* b) {
    return *a >= b;
  }

  Object* lt(Object* a, Object* b) {
    return *a < b;
  }

  Object* le(Object* a, Object* b) {
    return *a <= b;
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

  Object* newfunc(FunctionPtr* ref, unsigned int argc) {
    return new Function(ref, argc);
  }

  Object* call(Object* obj, int argc, ...) {
    va_list varargs;
    Object** argv = new Object*[argc];

    va_start(varargs, argc);
    for (int i = 0; i < argc; i++) {
      argv[i] = va_arg(varargs, Object*);
    }
    va_end(varargs);

    Object* ret = (*obj)(argc, argv);

    delete[] argv;
    return ret;
  }

  bool tobool(Object* obj) {
    return (bool)*obj;
  }

  void print(Object* obj) {
    puts((const char*)*obj);
  }

}