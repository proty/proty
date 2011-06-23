#include <stdio.h>
#include "runtime/runtime.h"
#include "file.h"

Object* io_print(Object* self, Object* obj) {
  if (obj->proto == String_proto) {
    printf("%s\n", obj->data.ptr);
  }
  else {
    Object* get_str = Object_getSlot(obj, "str");
    if (get_str != Qnil) {
      Object* string = Object_call(get_str, 1, obj);
      printf("%s\n", string->data.ptr);
    }
    else {
      printf("<%p>\n", obj);
    }
  }
  return Qnil;
}

Object* io_open(Object* self, Object* file, Object* mode) {
  if (file->proto == String_proto && mode->proto == String_proto) {
    Object* new = File_new(file->data.ptr, mode->data.ptr);
    return new;
  }
  else {
    return Qnil;
  }
}

Object* io_init() {
  File_proto = File_createProto();

  Object* io = Object_new(Object_proto);

  Object_setSlot(io, "print", Function_new((FuncPtr)io_print));
  Object_setSlot(io, "open", Function_new((FuncPtr)io_open));

  return io;
}
