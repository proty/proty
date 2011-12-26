#include <stdlib.h>
#include "runtime/runtime.h"

Object* os_system(Object* self, Object* command) {
    if (command->proto == String_proto) {
        int res = system(command->data.ptr);
        return Integer_new(res);
    }
    else {
        return Qnil;
    }
}

Object* os_getenv(Object* self, Object* name) {
    if (name->proto == String_proto) {
        return String_new(getenv(name->data.ptr));
    }
    else {
        return Qnil;
    }
}

Object* os_setenv(Object* self, Object* name, Object* value) {
    if (name->proto == String_proto && value->proto == String_proto) {
        setenv(name->data.ptr, value->data.ptr, 1);
        return Qtrue;
    }
    else {
        return Qfalse;
    }
}

Object* os_exit(Object* self, Object* status) {
    if (status->proto == Integer_proto) {
        exit(status->data.i);
        return Qtrue;
    }
    else {
        return Qfalse;
    }
}

Object* os_init() {
    Object* os = Object_new(Object_proto);

    Object_setSlot(os, SYM(system), FUNC(os_system, 2));
    Object_setSlot(os, SYM(getenv), FUNC(os_getenv, 2));
    Object_setSlot(os, SYM(setenv), FUNC(os_setenv, 3));
    Object_setSlot(os, SYM(exit), FUNC(os_exit, 2));

    return os;
}
