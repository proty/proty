#ifndef PROTY_SYMTAB_H
#define PROTY_SYMTAB_H

#include <runtime/runtime.h>

typedef struct SymTab {
    Object* symbols;
} SymTab;

SymTab* SymTab_new();
void SymTab_delete(SymTab* self);
void SymTab_store(SymTab* self, const char* name, int value);
int SymTab_lookup(SymTab* self, const char* name);

#endif
