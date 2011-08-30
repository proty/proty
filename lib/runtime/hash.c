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
 * Generates an integer hash of a string value. This function should
 * generate hashes fast and with a low collision rate.
 */

static unsigned int hash_str(const char* str) {
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

static unsigned int hash_addr(void* addr) {
  unsigned int key = (unsigned int)addr;
  return (key >> 3) * 2654435761;
}

/*
 * Invokes the matching hash function.
 */

static unsigned int hash_obj(Object* obj) {
  if (obj->proto == String_proto) {
    return hash_str(obj->data.ptr);
  }
  else if (obj->proto == Symbol_proto) {
    return hash_addr(obj);
  }
  else {
    return Object_send(obj, SYM(hash), 0)->data.i;
  }
}

/*
 * Tests objects for equality.
 */

static int cmp_obj(Object* a, Object* b) {
  if (a->proto == Symbol_proto) {
    return a == b;
  }
  else if (a->proto == String_proto) {
    if (b->proto == String_proto) {
      return strcmp(a->data.ptr, b->data.ptr) ? 0 : 1;
    }
    else {
      return 0;
    }
  }
  else {
    return Object_send(a, SYM(==), 1, b) == Qtrue;
  }
}

Object* Hash_init(Object* self) {
  Hash* hash = malloc(sizeof(Hash));

  hash->size = 0;
  hash->bounds = 8;

  hash->keys = calloc(sizeof(Object*), hash->bounds);
  hash->values = malloc(sizeof(Object*)*hash->bounds);

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
    int newBounds = hash->bounds * 2;

    Object** tmpKeys = calloc(sizeof(Object*), newBounds);
    Object** tmpValues = malloc(sizeof(Object*)*newBounds);

    // rehash each key
    for (int i = 0; i < hash->bounds; i++) {
      if (hash->keys[i]) {
        unsigned int pos = hash_obj(hash->keys[i]) % newBounds;
        while (tmpKeys[pos] && !cmp_obj(hash->keys[i], tmpKeys[pos])) {
          pos = (pos+1) % newBounds;
        }
        tmpKeys[pos] = hash->keys[i];
        tmpValues[pos] = hash->values[i];
      }
    }

    free(hash->keys);
    free(hash->values);

    hash->keys = tmpKeys;
    hash->values = tmpValues;

    hash->bounds = newBounds;
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

  Object_setSlot(Hash_proto, SYM(init), FUNC(Hash_init, 1));
  Object_setSlot(Hash_proto, SYM([]), FUNC(Hash_get, 2));
  Object_setSlot(Hash_proto, SYM([]=), FUNC(Hash_set, 3));
}
