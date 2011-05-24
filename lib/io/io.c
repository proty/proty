#include <stdio.h>
#include "runtime/runtime.h"

Object* io_print(Object* obj) {
  if (obj->proto == String_proto) {
    printf("%s\n", obj->data.ptr);
  }
  else {
    Object* get_str = Object_getSlot(obj, "str");
    if (get_str) {
      Object* string = Object_call(get_str, 1, obj);
      printf("%s\n", string->data.ptr);
    }
    else {
      printf("<%p>\n", obj);
    }
  }
  return 0;
}

Object* io_init() {
  Object* io = Object_new(Object_proto);

  Object_setSlot(io, "print", Function_new((FuncPtr)io_print));

  return io;
}
