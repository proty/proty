#include <stdlib.h>
#include "runtime.h"

typedef struct {
  int size;
  int bounds;
  Object** objects;
} List;

static inline void List_resize(List* list) {
  // if the list is full
  if (list->size == list->bounds) {
    int new_size = list->bounds + 1;
    new_size = (new_size >> 3) + (new_size < 9 ? 3 : 6) + new_size;
    list->objects = realloc(list->objects, new_size*sizeof(Object*));
    list->bounds = new_size;
  }
}

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
  List_resize(list);
  list->objects[list->size++] = obj;
  return obj;
}

Object* List_get(Object* self, Object* index) {
  List* list = self->data.ptr;
  if (index->data.i >= list->size) {
    /// @todo: throw exception
    return Qnil;
  }
  return list->objects[index->data.i];
}

Object* List_set(Object* self, Object* index, Object* value) {
  List* list = self->data.ptr;
  if (index->data.i >= list->size) {
    /// @todo: throw exception
    return Qnil;
  }
  list->objects[index->data.i] = value;
  return value;
}

Object* List_each(Object* self, Object* func) {
  List* list = self->data.ptr;
  for (int i = 0; i < list->size; i++) {
    Object_call(func, 2, self, list->objects[i]);
  }
  return Qnil;
}

Object* List_insert(Object* self, Object* index, Object* value) {
  List* list = self->data.ptr;
  if (index->data.i >= list->size) {
    /// @todo: throw exception
    return Qnil;
  }

  List_resize(list);

  for (int i = list->size; i > index->data.i; i--) {
    list->objects[i] = list->objects[i-1];
  }

  list->objects[index->data.i] = value;
  list->size++;

  return value;
}

Object* List_remove(Object* self, Object* index) {
  List* list = self->data.ptr;
  if (index->data.i >= list->size) {
    /// @todo: throw exception
    return Qnil;
  }

  Object* ret = list->objects[index->data.i];

  for (int i = index->data.i; i < list->size; i++) {
    list->objects[i] = list->objects[i+1];
  }

  list->size--;

  return ret;
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
  Object_setSlot(List_proto, "insert", FUNC(List_insert, 3));
  Object_setSlot(List_proto, "remove", FUNC(List_remove, 2));
  Object_setSlot(List_proto, "[]", FUNC(List_get, 2));
  Object_setSlot(List_proto, "[]=", FUNC(List_set, 3));
  Object_setSlot(List_proto, "each", FUNC(List_each, 2));
  Object_setSlot(List_proto, "size", FUNC(List_size, 1));
  Object_setSlot(List_proto, "bool", FUNC(List_bool, 1));
}
