#ifndef EXCEPTION_H
#define EXCEPTION_H

extern Object* Exception_proto;
void Exception_initProto();

Object* Exception_new(const char* str);
Object* Exception_raise(Object* self);

#endif
