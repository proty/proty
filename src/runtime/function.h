#ifndef FUNCTION_H
#define FUNCTION_H

extern Object* Function_proto;
void Function_initProto();

Object* Function_new(int id, int argc);
int Function_getId(Object* self);
int Function_getArgc(Object* self);
Object* Function_call(Object* self, Object* fself, int argc, Object* argv[]);

#endif
