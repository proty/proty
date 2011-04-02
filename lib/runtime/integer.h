#ifndef INTEGER_H
#define INTEGER_H

extern Object* Integer_proto;
Object* Integer_createProto();

Object* Integer_new(int);

Object* Integer_add(Object*, Object*);
Object* Integer_sub(Object*, Object*);
Object* Integer_mul(Object*, Object*);
Object* Integer_div(Object*, Object*);

#endif
