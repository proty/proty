#include <stdlib.h>
#include "runtime.h"

Object* Object_createProto() {
  Object* proto = malloc(sizeof(Object));
  return proto;
}

Object* Object_new(Object* proto) {
  Object* new = malloc(sizeof(Object));
  new->proto = proto;
  return new;
}

Object* Object_setSlot(Object* self, Object* key, Object* value) {
  Hash_set(self->slots, key, value);
  return 0;
}

Object* Object_getSlot(Object* self, Object* key) {
  Hash_get(self->slots, key);
  return 0;
}
