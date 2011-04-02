#ifndef OBJECT_H
#define OBJECT_H

struct Object {
  union {
    void*  ptr;
    int    i;
    double d;
  } data;

  Object* proto;
  Hash* slots;
};

extern Object* Object_proto;
Object* Object_createProto();

Object* Object_new(Object*);

Object* Object_setSlot(Object*, Symbol*, Object*);
Object* Object_getSlot(Object*, Symbol*);

#endif
