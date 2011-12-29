#include <stdlib.h>
#include "symtab.h"

SymTab* SymTab_new() {
    SymTab* self = malloc(sizeof(SymTab));
    self->symbols = Hash_new();
    return self;
}

void SymTab_delete(SymTab* self) {
    Hash_delete(self->symbols);
    free(self);
}

void SymTab_store(SymTab* self, const char* name, int value) {
    Object* sym = Symbol_get(name);
    Hash_set(self->symbols, sym, Integer_new(value));
}

int SymTab_lookup(SymTab* self, const char* name) {
    Object* sym = Symbol_get(name);
    Object* res = Hash_get(self->symbols, sym);
    return res == Qnil ? 0 : res->data.i;
}
