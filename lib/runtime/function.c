#include <stdlib.h>
#include <stdarg.h>
#include "runtime.h"

Object* Function_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, Symbol_get("()"), Function_new((FuncPtr)Function_call));

  return proto;
}

Object* Function_new((FuncPtr)func, int argc) {

}

Object* Function_call(Object* self, int argc, ...) {
  FuncPtr func = (FuncPtr)self->data.ptr;
  va_list varargs;

  Object** argv = malloc(sizeof(Object*)*argc);

  va_start(varargs, argc);
  for (int i = 0; i < argc; i++) {
    argv[i] = va_arg(varargs, Object*);
  }
  va_end(varargs);

  Object* ret;
  switch (argc) {
    case 0: ret = func(0); break;
    case 1: ret = func(argv[0]); break;
    case 2: ret = func(argv[0], argv[1]); break;
    case 3: ret = func(argv[0], argv[1], argv[2]); break;
    case 4: ret = func(argv[0], argv[1], argv[2], argv[3]); break;
    case 5: ret = func(argv[0], argv[1], argv[2], argv[3], argv[4]); break;
    case 6: ret = func(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]); break;
    case 7: ret = func(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]); break;
  }

  free(argv);

  return ret;
}
