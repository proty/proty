#include <stdlib.h>
#include "runtime.h"

Object* Hash_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, Symbol_get("set"), Function_new((FuncPtr)Hash_set));
  Object_setSlot(proto, Symbol_get("get"), Function_new((FuncPtr)Hash_get));

  return proto;
}

Object* Hash_init(Object* self) {
  Hash* hash = malloc(sizeof(Hash));

  hash->size = 0;
  hash->bounds = 8;

  hash->keys = malloc(sizeof(Symbol*)*hash->bounds);
  hash->values = malloc(sizeof(Object*)*hash->bounds);

  self->data.ptr = hash;
  return self;
}

Object* Hash_set(Object* self, Object* key, Object* value) {
  Hash* hash = self->data.ptr;

  if ((float)hash->size/hash->bounds > 0.75) {
    hash->bounds *= 2;
    hash->keys = realloc(hash->keys, hash->bounds*sizeof(Symbol*));
    hash->values = realloc(hash->values, hash->bounds*sizeof(Object*));
  }

  return 0;
}

Object* Hash_get(Object* self, Object* key) {
  return 0;
}
