#ifndef SYMBOL_H
#define SYMBOL_H

void SymbolTable_init();

extern Object* Symbol_proto;
void Symbol_initProto();

Object* Symbol_get(const char*);

#endif
