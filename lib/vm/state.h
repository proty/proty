#ifndef STATE_H
#define STATE_H

#include <runtime/runtime.h>

typedef int Reg;

typedef struct State {
    int sp;
    Object** registers;
    Object** stack;
} State;

State* State_new();

#endif
