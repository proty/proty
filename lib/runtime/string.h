#ifndef STRING_H
#define STRING_H

extern Object* String_proto;
void String_initProto();

Object* String_new(const char*);

#endif
