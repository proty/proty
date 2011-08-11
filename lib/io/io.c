#include <stdio.h>
#include "runtime/runtime.h"
#include "file.h"

MODFUNC(io_print, Object* self, Object* obj) {
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

MODFUNC(io_write, Object* self, Object* obj) {
  if (obj->proto == String_proto) {
    printf("%s", obj->data.ptr);
  }
  else {
    Object* get_str = Object_getSlot(obj, "str");
    if (get_str != Qnil) {
      Object* string = Object_call(get_str, 1, obj);
      printf("%s", string->data.ptr);
    }
    else {
      printf("<%p>", obj);
    }
  }
  return Qnil;
}

MODFUNC(io_open, Object* self, Object* file, Object* mode) {
  if (file->proto == String_proto && mode->proto == String_proto) {
    Object* new = File_new(file->data.ptr, mode->data.ptr);
    return new;
  }
  else {
    return Qnil;
  }
}

EXPORT(io_File);

void io_init() {
  File_proto = File_createProto();

  INITFUNC(io_write, 2);
  INITFUNC(io_print, 2);
  INITFUNC(io_open, 3);
  INITOBJ(io_File, File_proto);
}
