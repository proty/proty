#include <stdlib.h>
#include <stdio.h>
#include "runtime.h"
#include "vm/state.h"
#include "vm/eval.h"

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

Object* Function_call(Object* self, Object* fself, int argc, Object* argv[]) {
    Function* function = (Function*)self->data.ptr;

    /// @todo: throw exception
    if (argc != function->argc) {
        printf("proty: call with wrong argument count, expected %i, got %i\n",
               function->argc, argc);
        abort();
    }

    State* state = State_getGlobalState();

    for (int i = 0; i < argc; i++) {
        state->stack[state->sp++] = argv[i];
    }

    return eval(state, function->id);
}

int Function_getId(Object* self) {
    return ((Function*)self->data.ptr)->id;
}

int Function_getArgc(Object* self) {
    return ((Function*)self->data.ptr)->argc;
}

void Function_initProto() {
}
