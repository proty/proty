#ifndef STATE_H
#define STATE_H

#include <runtime/runtime.h>

typedef struct Module Module;
typedef int Reg;

typedef struct State {
    int sp;
    Object** registers;
    Object** stack;
    Module* module;
} State;

State* State_new(Module* module);
void State_delete(State* self);

#endif
