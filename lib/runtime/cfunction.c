#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "runtime.h"

typedef struct {
    FuncPtr func;
    int argc;
} CFunction;

Object* CFunction_new(FuncPtr func, int argc) {
    Object* new = Object_new(CFunction_proto);
    CFunction* f = malloc(sizeof(CFunction));

    f->func = func;
    f->argc = argc;

    new->data.ptr = f;
    return new;
}

Object* CFunction_call(Object* self, Object* fself, int argc, Object* argv[]) {
    CFunction* function = (CFunction*)self->data.ptr;

    /// @todo: throw exception
    if (argc != function->argc) {
        printf("proty: call with wrong argument count, expected %i, got %i\n", function->argc, argc);
        abort();
    }

    if (!fself) fself = Qnil;

    FuncPtr func = function->func;
    Object* ret;
    switch (argc) {
    case 0: ret = func(fself); break;
    case 1: ret = func(fself, argv[0]); break;
    case 2: ret = func(fself, argv[0], argv[1]); break;
    case 3: ret = func(fself, argv[0], argv[1], argv[2]); break;
    case 4: ret = func(fself, argv[0], argv[1], argv[2], argv[3]); break;
    case 5: ret = func(fself, argv[0], argv[1], argv[2], argv[3], argv[4]); break;
    case 6: ret = func(fself, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]); break;
    case 7: ret = func(fself, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]); break;
    }
    return ret;
}

void CFunction_initProto() {
}
