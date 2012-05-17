#include <stdio.h>
#include <stdlib.h>
#include "runtime/runtime.h"
#include "io/io.h"

Object* test_context;

Object* test_run(Object* self, Object* name, Object* function) {
    printf("testing: %s\n", (char*)name->data.ptr);
    test_context->data.i = 1;

    Object_call(function, 1, &test_context);

    if (test_context->data.i) printf("status: passed");
    else printf("status: failed");

    printf("\n---\n");

    return Qnil;
}

Object* test_assertEqual(Object* self, Object* a, Object* b) {
    Object* value = Object_send(a, SYM(==), 1, &b);
    if (value != Qtrue) {
        printf("assertion failed: ");
        io_write(0, a);
        printf(" != ");
        io_write(0, b);
        printf("\n");

        self->data.i = 0;
    }
    return Qnil;
}

Object* test_assertTrue(Object* self, Object* obj) {
    Object* boolean = Object_send(obj, SYM(bool), 0, 0);

    if (boolean != Qtrue) {
        printf("assertion failed: ");
        io_write(0, obj);
        printf(" is not true\n");

        self->data.i = 0;
    }
    return Qnil;
}

Object* test_assertFalse(Object* self, Object* obj) {
    Object* boolean = Object_send(obj, SYM(bool), 0, 0);

    if (boolean != Qfalse) {
        printf("assertion failed: ");
        io_write(0, obj);
        printf(" is not false\n");

        self->data.i = 0;
    }
    return Qnil;
}

Object* test_init() {
    test_context = Object_new(Object_proto);
    Object_setSlot(test_context, SYM(assert_equal), FUNC(test_assertEqual, 3));
    Object_setSlot(test_context, SYM(assert_true), FUNC(test_assertTrue, 2));
    Object_setSlot(test_context, SYM(assert_false), FUNC(test_assertFalse, 2));

    Object* test = Object_new(Object_proto);
    Object_setSlot(test, SYM(run), FUNC(test_run, 3));
    return test;
}
