#ifndef FUNCTION_H
#define FUNCTION_H

extern Object* Function_proto;
Object* Function_createProto();

Object* Function_new(FuncPtr);

#endif