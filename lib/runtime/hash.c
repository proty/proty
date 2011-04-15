#include <stdlib.h>
#include <string.h>
#include "runtime.h"

Object* Hash_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, Symbol_get("init", Function_new((FuncPtr)Hash_init));

  Object_setSlot(proto, Symbol_get("set"), Function_new((FuncPtr)Hash_set));
  Object_setSlot(proto, Symbol_get("get"), Function_new((FuncPtr)Hash_get));

  return proto;
}

Object* Hash_init(Object* self) {
  Hash* hash = malloc(sizeof(Hash));

  hash->size = 0;
  hash->bounds = 8;

  hash->keys = malloc(sizeof(Object*)*hash->bounds);
  hash->values = malloc(sizeof(Object*)*hash->bounds);

  memset(hash->keys, 0, hash->bounds*sizeof(Object*));

  self->data.ptr = hash;
  return self;
}

Object* Hash_set(Object* self, Object* key, Object* value) {
  Hash* hash = self->data.ptr;

  if ((float)hash->size/hash->bounds > 0.75) {
    Object** tmpKeys = malloc(sizeof(Object*)*hash->bounds*2);
    Object** tmpValues = malloc(sizeof(Object*)*hash->bounds*2);

    memset(tmpKeys, 0, sizeof(Object*)*hash->bounds*2);

    for (int i = 0; i < hash->bounds; i++) {
      if (hash->keys[i]) {
        unsigned int pos = (unsigned int)hash->keys[i] % hash->bounds*2;
        while (tmpKeys[pos] && (hash->keys[i] != tmpKeys[pos])) pos = (pos+1) % hash->bounds;
	tmpKeys[pos] = hash->keys[i];
	tmpValues[pos] = hash->values[i];
      }
    }

    free(hash->keys);
    free(hash->values);

    hash->keys = tmpKeys;
    hash->values = tmpValues;

    hash->bounds *= 2;
  }

  unsigned int pos = (unsigned int)key % hash->bounds;

  while (hash->keys[pos] && (hash->keys[pos] != key)) pos = (pos+1) % hash->bounds;

  hash->keys[pos] = key;
  hash->values[pos] = value;
  hash->size++;

  return 0;
}

Object* Hash_get(Object* self, Object* key) {
  Hash* hash = (Hash*)self->data.ptr;
  unsigned int pos = (unsigned int)key % hash->bounds;
  while (!hash->keys[pos] || hash->keys[pos] != key) pos = (pos+1) % hash->bounds;
  return hash->values[pos];
}
