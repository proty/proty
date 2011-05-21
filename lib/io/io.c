#include <stdio.h>
#include "runtime/runtime.h"

Object* io_print(Object* obj) {
  printf("%p\n", obj);
  return 0;
}

Object* io_init() {
  Object* io = Object_new(Object_proto);

  Object* print = Function_new((FuncPtr)io_print);
  Object_setSlot(io, "print", print);

  return io;
}
