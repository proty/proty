#include "runtime.h"
#include <stdio.h>

Object* Object_proto = 0;
Object* CFunction_proto = 0;
Object* Function_proto = 0;
Object* Hash_proto = 0;
Object* List_proto = 0;
Object* Symbol_proto = 0;
Object* Bool_proto = 0;
Object* Integer_proto = 0;
Object* Float_proto = 0;
Object* String_proto = 0;
Object* Exception_proto = 0;

Object* Qnil = 0;
Object* Qtrue = 0;
Object* Qfalse = 0;

void runtime_init() {
    Object_proto = Object_new(0);
    CFunction_proto = Object_new(Object_proto);
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
    CFunction_initProto();
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
