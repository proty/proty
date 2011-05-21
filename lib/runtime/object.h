#ifndef OBJECT_H
#define OBJECT_H

struct Object {
  union {
    void*  ptr;
    int    i;
    double d;
  } data;

  Object* proto;
  Object* slots;
};

extern Object* Object_proto;
Object* Object_createProto();

Object* Object_new(Object*);

Object* Object_setSlot(Object*, const char*, Object*);
Object* Object_getSlot(Object*, const char*);

Object* Object_call(Object*, int, ...);

#endif
