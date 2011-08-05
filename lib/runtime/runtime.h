#ifndef RUNTIME_H
#define RUNTIME_H

typedef struct Object Object;
typedef Object* (*FuncPtr)(Object*, ...);

#include "object.h"
#include "hash.h"
#include "list.h"
#include "symbol.h"
#include "function.h"
#include "integer.h"
#include "float.h"
#include "string.h"
#include "nil.h"
#include "bool.h"
#include "exception.h"

#define FUNC(name, argc) Function_new((FuncPtr)name, argc)
#define SYM(name) Symbol_get(#name)

#define MODFUNC(name, ...) Object* prmod_##name;    \
  Object* name(__VA_ARGS__)
#define EXPORT(name) Object* prmod_##name

#define MODINIT(name, obj) prmod_##name = obj
#define MODGET(name) prmod_##name

#endif
