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
    int new_size = list->bounds + 1;
    new_size = (new_size >> 3) + (new_size < 9 ? 3 : 6) + new_size;
    list->objects = realloc(list->objects, new_size*sizeof(Object*));
    list->bounds = new_size;
  }

  list->objects[list->size++] = obj;

  return obj;
}

Object* List_get(Object* self, Object* index) {
  List* list = self->data.ptr;
  return list->objects[index->data.i];
}

Object* List_set(Object* self, Object* index, Object* value) {
  List* list = self->data.ptr;
  if (index->data.i > list->size) {
    /// @todo: throw exception
    return Qnil;
  }
  list->objects[index->data.i] = value;
  return value;
}

Object* List_size(Object* self) {
  List* list = self->data.ptr;
  return Integer_new(list->size);
}

Object* List_bool(Object* self) {
  List* list = self->data.ptr;
  return list->size > 0 ? Qtrue : Qfalse;
}

void List_initProto() {
  Object_setSlot(List_proto, "append", FUNC(List_append, 2));
  Object_setSlot(List_proto, "[]", FUNC(List_get, 2));
  Object_setSlot(List_proto, "[]=", FUNC(List_set, 3));
  Object_setSlot(List_proto, "size", FUNC(List_size, 1));
  Object_setSlot(List_proto, "bool", FUNC(List_bool, 1));
}
