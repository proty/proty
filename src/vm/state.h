#ifndef STATE_H
#define STATE_H

typedef int Reg;
struct Object;
struct Module;

typedef struct State {
    int sp;
    struct Object** registers;
    struct Object** stack;
    struct Object* exception;
    struct Module* module;
} State;

State* State_new(struct Module* module);
void State_delete(State* self);

void State_setGlobalState(State* state);
State* State_getGlobalState();

#endif
