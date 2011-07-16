#include <stdlib.h>
#include "runtime.h"

typedef struct {
  int size;
  int bounds;
  Object** objects;
} List;

Object* List_init(Object* self) {
  List* list = malloc(sizeof(List));

  list->size = 0;
  list->bounds = 0;
  list->objects = 0;

  self->data.ptr = list;
  return self;
}

Object* List_new() {
  Object* new = Object_new(List_proto);
  List_init(new);
  return new;
}

Object* List_append(Object* self, Object* obj) {
  List* list = self->data.ptr;

  // resize if the list is full
  if (list->size == list->bounds) {
    list->bounds = (list->bounds*1.15) + 3;
    list->objects = realloc(list->objects,
                            list->bounds*sizeof(Object*));

  }

  list->objects[list->size++] = obj;

  return obj;
}

Object* List_get(Object* self, Object* pos) {
  List* list = self->data.ptr;
  return list->objects[pos->data.i];
}

Object* List_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "append", FUNC(List_append, 2));
  Object_setSlot(proto, "get", FUNC(List_get, 2));

  return proto;
}
