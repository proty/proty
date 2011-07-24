#ifndef HASH_H
#define HASH_H

extern Object* Hash_proto;
void Hash_initProto();

Object* Hash_new();

Object* Hash_set(Object*, Object*, Object*);
Object* Hash_get(Object*, Object*);

#endif
