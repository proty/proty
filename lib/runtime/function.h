#ifndef FUNCTION_H
#define FUNCTION_H

extern Object* Function_proto;
void Function_initProto();

Object* Function_new(FuncPtr, int);

Object* Function_call(Object*, int argc, Object*[]);

#endif
