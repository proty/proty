#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

Object* File_read(Object* self, Object* len) {
  char* in = malloc(len->data.i);
  fread(in, 1, len->data.i, self->data.ptr);
  return String_new(in);
}

Object* File_createProto() {
  Object* file = Object_new(Object_proto);

  Object_setSlot(file, "close", Function_new((FuncPtr)File_close, 1));
  Object_setSlot(file, "write", Function_new((FuncPtr)File_write, 2));
  Object_setSlot(file, "read", Function_new((FuncPtr)File_read, 2));

  return file;
}
