#include <stdlib.h>
#include "state.h"

State* State_new() {
    State* state = malloc(sizeof(State));
    state->sp = 0;
    state->registers = malloc(sizeof(Object*)*256);
    state->stack = malloc(sizeof(Object*)*256);
    return state;
}
