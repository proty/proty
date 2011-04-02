#ifndef HASH_H
#define HASH_H

struct Hash {
  int size;
  int bounds;
  Symbol** keys;
  Object** values;
};

extern Object* Hash_proto;
Object* Hash_createProto();

Object* Hash_set(Hash*, Symbol*, Object*);
Object* Hash_get(Hash*, Symbol*);

#endif
