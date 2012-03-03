#ifndef STATE_H
#define STATE_H

typedef struct Object_t Object;
typedef struct Module Module;
typedef int Reg;

typedef struct State {
    int sp;
    Object** registers;
    Object** stack;
    Object* exception;
    Module* module;
} State;

State* State_new(Module* module);
void State_delete(State* self);

void State_setGlobalState(State* state);
State* State_getGlobalState();

#endif
