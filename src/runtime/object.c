#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "runtime.h"

Object* Object_new(Object* proto) {
    Object* new = malloc(sizeof(Object));
    new->proto = proto;
    new->slots = 0;
    return new;
}

Object* Object_delete(Object* self) {
    if (self->slots) free(self->slots);
    free(self);
    return Qnil;
}

Object* Object_setSlot(Object* self, Object* key, Object* value) {
    if (!self->slots) self->slots = Hash_new();
    return Hash_set(self->slots, key, value);
}

Object* Object_getSlot(Object* self, Object* key) {
    Object* proto = self;

    while (1) {
        if (proto->slots) {
            Object* value = Hash_get(proto->slots, key);
            if (value != Qnil) return value;
        }

        if (proto->proto) proto = proto->proto;
        else break;
    }

    return Qnil;
}

Object* Object_call(Object* self, int argc, Object* argv[]) {
    if (self->proto == CFunction_proto) {
        return CFunction_call(self, 0, argc, argv);
    }
    else if (self->proto == Function_proto) {
        return Function_call(self, 0, argc, argv);
    }
    else {
        /// @todo: throw exception
        printf("proty: called non-function object\n");
        abort();
        return 0;
    }
}

Object* Object_send(Object* self, Object* key, int argc, Object* argv[]) {
    Object* func = Object_getSlot(self, key);

    if (func->proto == CFunction_proto) {
        return CFunction_call(func, self, argc, argv);
    }
    else if (func->proto == Function_proto) {
        return Function_call(func, self, argc, argv);
    }
    else {
        /// @todo: throw exception
        printf("proty: called non-function object\n");
        abort();
        return 0;
    }

    if (func->proto != CFunction_proto) {
        printf("proty: called non-function object\n");
        abort();
    }

}

Object* Object_str(Object* self) {
    return String_new("<Object>");
}

Object* Object_bool(Object* self) {
    return (self == Qfalse) ? Qfalse : Qtrue;
}

Object* Object_not(Object* self) {
    Object* bool = Object_send(self, SYM(bool), 0, 0);
    return bool == Qtrue ? Qfalse : Qtrue;
}

void Object_initProto() {
    Object_setSlot(Object_proto, SYM(new), FUNC(Object_new, 1));
    Object_setSlot(Object_proto, SYM(str), FUNC(Object_str, 1));
    Object_setSlot(Object_proto, SYM(bool), FUNC(Object_bool, 1));
    Object_setSlot(Object_proto, SYM(!), FUNC(Object_not, 1));
}
