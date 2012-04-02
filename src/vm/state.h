#ifndef STATE_H
#define STATE_H

#include <setjmp.h>

typedef int Reg;
struct Object;
struct Module;

typedef struct State {
    int sp;
    struct Object** registers;
    struct Object** stack;
    struct Module* module;

    jmp_buf excp_buf;
    struct Object* exception;
} State;

State* State_new(struct Module* module);
void State_delete(State* self);

void State_setGlobalState(State* state);
State* State_getGlobalState();

#endif
