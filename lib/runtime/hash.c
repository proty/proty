#include "runtime.h"

Object* Hash_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, Symbol_get("set"), Function_new((FuncPtr)Hash_set));
  Object_setSlot(proto, Symbol_get("get"), Function_new((FuncPtr)Hash_get));

  return proto;
}

Object* Hash_init(Object* self) {

}

Object* Hash_set(Object* self, Object* key, Object* value) {

}

Object* Hash_get(Object* self, Object* key) {

}
