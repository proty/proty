#include "runtime.h"
#include <stdio.h>

Object* Object_proto;
Object* Function_proto;
Object* Hash_proto;
Object* List_proto;
Object* Symbol_proto;
Object* Bool_proto;
Object* Integer_proto;
Object* Float_proto;
Object* String_proto;
Object* Exception_proto;

Object* Qnil;
Object* Qtrue;
Object* Qfalse;

void runtime_init() {
  Object_proto = Object_new(0);
  Function_proto = Object_new(Object_proto);
  Hash_proto = Object_new(Object_proto);
  List_proto = Object_new(Object_proto);
  Symbol_proto = Object_new(Object_proto);
  Bool_proto = Object_new(Object_proto);
  Integer_proto = Object_new(Object_proto);
  Float_proto = Object_new(Object_proto);
  String_proto = Object_new(Object_proto);
  Exception_proto = Object_new(Object_proto);

  SymbolTable_init();
  Object_initProto();
  Function_initProto();
  Hash_initProto();
  List_initProto();
  Symbol_initProto();
  Bool_initProto();
  Integer_initProto();
  Float_initProto();
  String_initProto();
  Exception_initProto();

  Qnil = Nil_new();
  Qtrue = Bool_new();
  Qfalse = Bool_new();
}
