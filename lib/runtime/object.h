#ifndef OBJECT_H
#define OBJECT_H

struct Object_t {
  union {
    void*  ptr;
    int    i;
    double d;
  } data;

  Object* proto;
  Object* slots;
};

extern Object* Object_proto;
void Object_initProto();

Object* Object_new(Object*);

Object* Object_setSlot(Object*, const char*, Object*);
Object* Object_getSlot(Object*, const char*);

Object* Object_call(Object*, int, ...);

#endif
