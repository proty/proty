#include "runtime.h"

Object* Object_proto;
Object* Symbol_proto;
Object* Function_proto;
Object* Hash_proto;

void runtime_init() {
  Object_proto = Object_createProto();
  Symbol_proto = Symbol_createProto();
  Function_proto = Function_createProto();
  Hash_proto = Hash_createProto();
}
