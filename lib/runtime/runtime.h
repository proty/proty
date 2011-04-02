#ifndef RUNTIME_H
#define RUNTIME_H

typedef struct Object Object;
typedef struct Symbol Symbol;
typedef struct Function Function;
typedef struct Hash Hash;

typedef Object* (*FuncPtr)(Object*, ...);

#include "object.h"
#include "symbol.h"
#include "hash.h"
#include "function.h"

#endif
