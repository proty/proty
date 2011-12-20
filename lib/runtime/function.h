#ifndef FUNCTION_H
#define FUNCTION_H

extern Object* Function_proto;
void Function_initProto();

Object* Function_new(FuncPtr ptr, int argc);

Object* Function_call(Object* self, Object* fself, int argc, Object*[]);

#endif
