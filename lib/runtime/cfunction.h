#ifndef CFUNCTION_H
#define CFUNCTION_H

extern Object* CFunction_proto;
void CFunction_initProto();

Object* CFunction_new(FuncPtr ptr, int argc);

Object* CFunction_call(Object* self, Object* fself, int argc, Object*[]);

#endif
