#ifndef STATE_H
#define STATE_H

#include <setjmp.h>

typedef struct Object_t Object;
typedef struct Module Module;
typedef int Reg;

typedef struct State {
    int sp;
    Object** registers;
    Object** stack;
    Module* module;

    jmp_buf excp_buf;
    Object* exception;
} State;

State* State_new(Module* module);
void State_delete(State* self);

void State_setGlobalState(State* state);
State* State_getGlobalState();

#endif
