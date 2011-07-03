#ifndef HASH_H
#define HASH_H

struct Hash {
  int size;
  int bounds;
  const char** keys;
  Object** values;
};

extern Object* Hash_proto;
Object* Hash_createProto();
Object* Hash_new();

Object* Hash_set(Object*, const char*, Object*);
Object* Hash_get(Object*, const char*);

#endif
