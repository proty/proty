#include <stdio.h>
#include <stdlib.h>
#include "runtime/runtime.h"

Object* test_context;

Object* test_run(Object* self, Object* name, Object* function) {
    Object_call(function, 1, &test_context);
    return Qnil;
}

Object* test_assert(Object* self, Object* value) {
    if (value == Qtrue) printf("+");
    else printf("-");
    return Qnil;
}

Object* test_assertEqual(Object* self, Object* a, Object* b) {
    Object* value = Object_send(a, SYM(==), 1, &b);
    if (value == Qtrue) printf("+");
    else printf("-");
    return Qnil;
}

Object* test_assertTrue(Object* self, Object* obj) {
    if (obj == Qtrue) printf("+");
    else printf("-");
    return Qnil;
}

Object* test_assertFalse(Object* self, Object* obj) {
    if (obj == Qfalse) printf("+");
    else printf("-");
    return Qnil;
}

Object* test_init() {
    test_context = Object_new(Object_proto);
    Object_setSlot(test_context, SYM(assert), FUNC(test_assert, 2));
    Object_setSlot(test_context, SYM(assertEqual), FUNC(test_assertEqual, 3));
    Object_setSlot(test_context, SYM(assertTrue), FUNC(test_assertTrue, 2));
    Object_setSlot(test_context, SYM(assertFalse), FUNC(test_assertFalse, 2));

    Object* test = Object_new(Object_proto);
    Object_setSlot(test, SYM(run), FUNC(test_run, 3));
    return test;
}
