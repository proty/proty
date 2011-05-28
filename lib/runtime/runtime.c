#include "runtime.h"
#include <stdio.h>

Object* Object_proto;
Object* Function_proto;
Object* Hash_proto;
Object* Integer_proto;
Object* String_proto;

Object* Qnil;
Object* Qtrue;
Object* Qfalse;

void runtime_init() {
  Object_proto = Object_createProto();
  Function_proto = Function_createProto();
  Hash_proto = Hash_createProto();
  Integer_proto = Integer_createProto();
  String_proto = String_createProto();

  Qnil = Nil_create();
  Qtrue = Bool_create();
  Qfalse = Bool_create();
}
