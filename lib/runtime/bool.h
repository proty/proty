#ifndef BOOL_H
#define BOOL_H

extern Object* Bool_proto;
extern Object* Qtrue;
extern Object* Qfalse;

void Bool_initProto();
Object* Bool_new();

#endif
