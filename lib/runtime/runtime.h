#ifndef RUNTIME_H
#define RUNTIME_H

typedef struct Object Object;
typedef struct Hash Hash;

typedef Object* (*FuncPtr)(Object*, ...);

#include "object.h"
#include "hash.h"
#include "function.h"
#include "integer.h"
#include "string.h"

#endif
