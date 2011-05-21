#include <stdlib.h>
#include <string.h>
#include "runtime.h"

unsigned int hash_str(const char* str) {
  unsigned int hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

Object* Hash_createProto() {
  Object* proto = Object_new(Object_proto);

  // bootstrap hash prototype
  Hash_init(proto);
  proto->slots = Object_new(proto);
  Hash_init(proto->slots);

  Object_setSlot(proto, "init", Function_new((FuncPtr)Hash_init));
  Object_setSlot(proto, "set", Function_new((FuncPtr)Hash_set));
  Object_setSlot(proto, "get", Function_new((FuncPtr)Hash_get));

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

Object* Hash_set(Object* self, const char* key, Object* value) {
  Hash* hash = self->data.ptr;

  if ((float)hash->size/hash->bounds > 0.75) {
    const char** tmpKeys = malloc(sizeof(Object*)*hash->bounds*2);
    Object** tmpValues = malloc(sizeof(Object*)*hash->bounds*2);

    memset(tmpKeys, 0, sizeof(Object*)*hash->bounds*2);

    for (int i = 0; i < hash->bounds; i++) {
      if (hash->keys[i]) {
        unsigned int pos = hash_str(hash->keys[i]) % hash->bounds*2;
        while (tmpKeys[pos] && !strcmp(hash->keys[i], tmpKeys[pos])) pos = (pos+1) % hash->bounds;
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

  unsigned int pos = hash_str(key) % hash->bounds;

  while (hash->keys[pos] && strcmp(hash->keys[pos], key)) pos = (pos+1) % hash->bounds;

  hash->keys[pos] = key;
  hash->values[pos] = value;
  hash->size++;

  return 0;
}

Object* Hash_get(Object* self, const char* key) {
  Hash* hash = (Hash*)self->data.ptr;
  unsigned int pos = hash_str(key) % hash->bounds;
  while (hash->keys[pos] && strcmp(hash->keys[pos], key)) pos = (pos+1) % hash->bounds;
  return hash->values[pos];
}
