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
void Object_initProto();

Object* Object_new(Object* proto);
Object* Object_delete(Object* self);

Object* Object_setSlot(Object*, Object*, Object*);
Object* Object_getSlot(Object*, Object*);

Object* Object_send(Object*, Object*, int, Object*[]);
Object* Object_call(Object*, int, Object*[]);

#endif
