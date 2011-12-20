#include <stdio.h>
#include "runtime/runtime.h"
#include "file.h"

Object* io_write(Object* self, Object* obj) {
  if (obj->proto == String_proto) {
    printf("%s", obj->data.ptr);
  }
  else {
    Object* string = Object_send(obj, SYM(str), 0, 0);
    if (string != Qnil) {
      printf("%s", string->data.ptr);
    }
    else {
      printf("<%p>", obj);
    }
  }
  return Qnil;
}

Object* io_print(Object* self, Object* obj) {
  Object* ret = io_write(self, obj);
  printf("\n");
  return ret;
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

  Object_setSlot(io, SYM(write), FUNC(io_write, 1));
  Object_setSlot(io, SYM(print), FUNC(io_print, 1));
  Object_setSlot(io, SYM(open), FUNC(io_open, 2));
  Object_setSlot(io, SYM(File), File_proto);

  return io;
}
