#include <stdlib.h>
#include "symtab.h"

#define MAX_SCOPE_DEPTH 20

SymTab* SymTab_new() {
    SymTab* self = malloc(sizeof(SymTab));
    self->scopes = malloc(sizeof(Object*) * MAX_SCOPE_DEPTH);
    self->scopes[0] = Hash_new();
    self->scope = 0;
    return self;
}

void SymTab_delete(SymTab* self) {
    for (int i = self->scope; i >= 0; i--) {
        Hash_delete(self->scopes[i]);
    }
    free(self->scopes);
    free(self);
}

void SymTab_enterScope(SymTab* self) {
    self->scope++;
    if (self->scope == MAX_SCOPE_DEPTH) abort();
    self->scopes[self->scope] = Hash_new();
}

void SymTab_leaveScope(SymTab* self) {
    if (self->scope == 0) abort();
    Hash_delete(self->scopes[self->scope--]);
}

void SymTab_store(SymTab* self, const char* name, int value) {
    Object* sym = Symbol_get(name);
    Hash_set(self->scopes[self->scope], sym, Integer_new(value));
}

int SymTab_lookup(SymTab* self, const char* name) {
    Object* sym = Symbol_get(name);
    for (int i = self->scope; i >= 0; i--) {
        Object* res = Hash_get(self->scopes[i], sym);
        if (res != Qnil) return res->data.i;
    }
    return -1;
}
