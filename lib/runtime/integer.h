#ifndef INTEGER_H
#define INTEGER_H

extern Object* Integer_proto;
Object* Integer_createProto();

Object* Integer_new(int);

Object* Integer_str(Object*);
Object* Integer_bool(Object*);

Object* Integer_add(Object*, Object*);
Object* Integer_sub(Object*, Object*);
Object* Integer_mul(Object*, Object*);
Object* Integer_div(Object*, Object*);

Object* Integer_eq(Object*, Object*);
Object* Integer_ne(Object*, Object*);
Object* Integer_lt(Object*, Object*);
Object* Integer_gt(Object*, Object*);
Object* Integer_le(Object*, Object*);
Object* Integer_ge(Object*, Object*);

#endif
