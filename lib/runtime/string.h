#ifndef STRING_H
#define STRING_H

extern Object* String_proto;
Object* String_createProto();

Object* String_new(const char*);

Object* String_add(Object*, Object*);

#endif
