#include <stdlib.h>
#include "runtime/runtime.h"
#include "state.h"
#include "module.h"

static State* global_state = 0;

State* State_new(Module* module) {
    State* state = malloc(sizeof(State));
    state->sp = 0;
    state->registers = malloc(sizeof(Object*)*512);
    state->stack = malloc(sizeof(Object*)*256);
    state->module = module;
    return state;
}

void State_delete(State* self) {
    free(self->registers);
    free(self->stack);
    free(self);

    if (self == global_state) {
        global_state = 0;
    }
}

void State_setGlobalState(State* state) {
    global_state = state;
}

State* State_getGlobalState() {
    return global_state;
}
