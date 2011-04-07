#ifndef SYMBOL_H
#define SYMBOL_H

extern Object* Symbol_proto;
Object* Symbol_createProto();

Object* Symbol_get(const char*);

#endif
