#include "runtime.h"

Object* Object_proto;
Object* Function_proto;
Object* Hash_proto;
Object* Integer_proto;
Object* String_proto;

void runtime_init() {
  Object_proto = Object_createProto();
  Function_proto = Function_createProto();
  Hash_proto = Hash_createProto();
  Integer_proto = Integer_createProto();
  String_proto = String_createProto();
}
