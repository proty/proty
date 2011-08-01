#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "runtime.h"

typedef struct {
  int size;
  int bounds;
  Object** keys;
  Object** values;
} Hash;

/*
 * Generates an integer hash of a string value.
 * This function should generate hashes
 * fast and with a low collision rate.
 */

unsigned int hash_str(const char* str) {
  unsigned int hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

/*
 * Generates a hash of a memory address.
 * http://www.concentric.net/~ttwang/tech/addrhash.htm
 */

unsigned int hash_addr(void* addr) {
  unsigned int key = (unsigned int)addr;
  return (key >> 3) * 2654435761;
}

/*
 * Invokes the matching hash function.
 */

unsigned int hash_obj(Object* obj) {
  if (obj->proto == String_proto) {
    return hash_str(obj->data.ptr);
  }
  else if (obj->proto == Symbol_proto) {
    return hash_addr(obj);
  }
  else {
    Object* h = Object_getSlot(obj, "hash");
    return Object_call(h, 1, obj)->data.i;
  }
}

/*
 * Tests objects for equality.
 */

int cmp_obj(Object* a, Object* b) {
  if (a->proto == String_proto) {
    if (b->proto == String_proto) {
      return strcmp(a->data.ptr, b->data.ptr) ? 0 : 1;
    }
    else {
      return 0;
    }
  }
  else if (a->proto == Symbol_proto) {
    return a == b;
  }
  else {
    Object* h = Object_getSlot(a, "==");
    return Object_call(h, 2, a, b) == Qtrue;
  }
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

Object* Hash_new() {
  Object* new = Object_new(Hash_proto);
  Hash_init(new);
  return new;
}

Object* Hash_set(Object* self, Object* key, Object* value) {
  Hash* hash = self->data.ptr;

  // double the hash bounds if the hash is more than 75% full
  if ((hash->size*100)/hash->bounds > 75) {
    Object** tmpKeys = malloc(sizeof(Object*)*hash->bounds*2);
    Object** tmpValues = malloc(sizeof(Object*)*hash->bounds*2);

    memset(tmpKeys, 0, sizeof(Object*)*hash->bounds*2);

    // rehash each key
    for (int i = 0; i < hash->bounds; i++) {
      if (hash->keys[i]) {
        unsigned int pos = hash_obj(hash->keys[i]) % (hash->bounds*2);
        while (tmpKeys[pos] && !cmp_obj(hash->keys[i], tmpKeys[pos])) {
          pos = (pos+1) % hash->bounds;
        }
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

  unsigned int pos = hash_obj(key) % hash->bounds;

  while (hash->keys[pos] && !cmp_obj(hash->keys[pos], key)) {
    pos = (pos+1) % hash->bounds;
  }

  hash->keys[pos] = key;
  hash->values[pos] = value;
  hash->size++;

  return value;
}

Object* Hash_get(Object* self, Object* key) {
  Hash* hash = (Hash*)self->data.ptr;
  unsigned int pos = hash_obj(key) % hash->bounds;
  while (hash->keys[pos]) {
    if (!cmp_obj(hash->keys[pos], key)) {
      pos = (pos+1) % hash->bounds;
    }
    else {
      return hash->values[pos];
    }
  }
  return Qnil;
}

void Hash_initProto() {
  // bootstrap hash prototype
  Hash_proto->slots = Object_new(Hash_proto);
  Hash_init(Hash_proto->slots);

  Object_setSlot(Hash_proto, "init", FUNC(Hash_init, 1));
  Object_setSlot(Hash_proto, "[]", FUNC(Hash_get, 2));
  Object_setSlot(Hash_proto, "[]=", FUNC(Hash_set, 3));
}
