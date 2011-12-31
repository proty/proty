#include <stdlib.h>
#include "runtime.h"

typedef struct {
    int id;
    int argc;
} Function;

Object* Function_new(int id, int argc) {
    Object* new = Object_new(Function_proto);
    Function* f = malloc(sizeof(Function));

    f->id = id;
    f->argc = argc;

    new->data.ptr = f;
    return new;
}

int Function_getId(Object* self) {
    return ((Function*)self->data.ptr)->id;
}

void Function_initProto() {
}
