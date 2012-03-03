#include <stdlib.h>
#include "state.h"
#include "module.h"

State* State_new(Module* module) {
    State* state = malloc(sizeof(State));
    state->sp = 0;
    state->registers = malloc(sizeof(Object*)*256);
    state->stack = malloc(sizeof(Object*)*256);
    state->module = module;
    return state;
}

void State_delete(State* self) {
    free(self->registers);
    free(self->stack);
    free(self);
}
