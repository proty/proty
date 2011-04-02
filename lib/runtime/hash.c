#include "runtime.h"

Object* Hash_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, Symbol_get("set"), Function_new((FuncPtr)Hash_set));
  Object_setSlot(proto, Symbol_get("get"), Function_new((FuncPtr)Hash_get));

  return proto;
}
