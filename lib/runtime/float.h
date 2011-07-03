#ifndef FLOAT_H
#define FLOAT_H

extern Object* Float_proto;
Object* Float_createProto();

Object* Float_new(double);

Object* Float_str(Object*);
Object* Float_bool(Object*);

Object* Float_add(Object*, Object*);
Object* Float_sub(Object*, Object*);
Object* Float_mul(Object*, Object*);
Object* Float_div(Object*, Object*);

Object* Float_eq(Object*, Object*);
Object* Float_ne(Object*, Object*);
Object* Float_lt(Object*, Object*);
Object* Float_gt(Object*, Object*);
Object* Float_le(Object*, Object*);
Object* Float_ge(Object*, Object*);

#endif
