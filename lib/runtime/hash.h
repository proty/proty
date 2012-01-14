#ifndef HASH_H
#define HASH_H

extern Object* Hash_proto;
void Hash_initProto();

Object* Hash_new();
Object* Hash_delete(Object* self);

Object* Hash_set(Object* self, Object* key, Object* value);
Object* Hash_get(Object* self, Object* key);

#endif
