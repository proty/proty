#ifndef LIST_H
#define LIST_H

extern Object* List_proto;
void List_initProto();

Object* List_new();
Object* List_append(Object*, Object*);

#endif
