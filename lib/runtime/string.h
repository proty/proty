#ifndef STRING_H
#define STRING_H

extern Object* String_proto;
void String_initProto();

Object* String_new(const char*);
Object* String_add(Object* self, Object* other);
Object* String_iadd(Object* self, Object* other);
Object* String_eq(Object* self, Object* other);
Object* String_ne(Object* self, Object* other);
Object* String_bool(Object* self);
Object* String_split(Object* self, Object* delimiter);
Object* String_length(Object* self);

#endif
