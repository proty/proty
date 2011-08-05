#include <stdlib.h>
#include "runtime/runtime.h"

MODFUNC(os_system, Object* self, Object* command) {
  if (command->proto == String_proto) {
    int res = system(command->data.ptr);
    return Integer_new(res);
  }
  else {
    return Qnil;
  }
}

MODFUNC(os_getenv, Object* self, Object* name) {
  if (name->proto == String_proto) {
    return String_new(getenv(name->data.ptr));
  }
  else {
    return Qnil;
  }
}

MODFUNC(os_setenv, Object* self, Object* name, Object* value) {
  if (name->proto == String_proto && value->proto == String_proto) {
    setenv(name->data.ptr, value->data.ptr, 1);
    return Qtrue;
  }
  else {
    return Qfalse;
  }
}

MODFUNC(os_exit, Object* self, Object* status) {
  if (status->proto == Integer_proto) {
    exit(status->data.i);
    return Qtrue;
  }
  else {
    return Qfalse;
  }
}

void os_init() {
  INITFUNC(os_system, 2);
  INITFUNC(os_getenv, 2);
  INITFUNC(os_setenv, 3);
  INITFUNC(os_exit, 2);
}
