#ifndef RUNTIME_H
#define RUNTIME_H

typedef struct Object_t Object;
typedef Object* (*FuncPtr)(Object*, ...);

#include "object.h"
#include "hash.h"
#include "list.h"
#include "symbol.h"
#include "cfunction.h"
#include "function.h"
#include "integer.h"
#include "float.h"
#include "string.h"
#include "nil.h"
#include "bool.h"
#include "exception.h"
#include "vm/state.h"

#define FUNC(name, argc) CFunction_new((FuncPtr)name, argc-1)
#define SYM(name) Symbol_get(#name)

void runtime_init();

#endif
