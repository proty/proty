#include <stdio.h>
#include <string.h>
#include "runtime/runtime.h"

Object* File_proto;

Object* File_new(const char* file, const char* mode) {
  Object* new = Object_new(File_proto);
  new->data.ptr = fopen(file, mode);
  return new;
}

Object* File_close(Object* self) {
  int res = fclose(self->data.ptr);
  return res ? Qtrue : Qfalse;
}

Object* File_write(Object* self, Object* str) {
  if (str->proto == String_proto) {
    fwrite(str->data.ptr, 1, strlen(str->data.ptr), self->data.ptr);
  }
  return Qnil;
}

Object* File_createProto() {
  Object* file = Object_new(Object_proto);

  Object_setSlot(file, "close", Function_new((FuncPtr)File_close));
  Object_setSlot(file, "write", Function_new((FuncPtr)File_write));

  return file;
}
