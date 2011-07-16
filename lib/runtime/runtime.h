#ifndef RUNTIME_H
#define RUNTIME_H

typedef struct Object Object;
typedef Object* (*FuncPtr)(Object*, ...);

#include "object.h"
#include "hash.h"
#include "list.h"
#include "function.h"
#include "integer.h"
#include "float.h"
#include "string.h"
#include "nil.h"
#include "bool.h"
#include "exception.h"

#define FUNC(name, argc) Function_new((FuncPtr)name, argc)

#endif
