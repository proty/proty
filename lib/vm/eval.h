#ifndef PROTY_EVAL_H
#define PROTY_EVAL_H

#include <runtime/runtime.h>
#include "block.h"
#include "state.h"

Object* eval(State* state, Block* block);

#endif
