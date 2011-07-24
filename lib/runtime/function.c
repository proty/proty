#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "runtime.h"

typedef struct {
  FuncPtr func;
  int argc;
} Function;

Object* Function_new(FuncPtr func, int argc) {
  Object* new = Object_new(Function_proto);
  Function* f = malloc(sizeof(Function));

  f->func = func;
  f->argc = argc;

  new->data.ptr = f;
  return new;
}

Object* Function_call(Object* self, int argc, Object* argv[]) {
  Function* function = (Function*)self->data.ptr;

  /// @todo: throw exception
  if (argc != function->argc) {
    printf("wrong argument count, expected %i, got %i", function->argc, argc);
    exit(0);
  }

  FuncPtr func = function->func;
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
  return ret;
}

void Function_initProto() {
}
