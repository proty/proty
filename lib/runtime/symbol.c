#include <string.h>
#include <stdlib.h>
#include "runtime.h"

typedef struct {
  int size;
  int bounds;
  const char** keys;
  Object** values;
} Symtab;

Symtab* symtab;

void SymbolTable_init() {
  symtab = malloc(sizeof(Symtab));
  symtab->keys = calloc(sizeof(char*), 64);
  symtab->values = malloc(sizeof(Object*)*64);

  symtab->bounds = 64;
  symtab->size = 0;
}

/*
 * Generates an integer hash of a string value. This function should
 * generate hashes fast and with a low collision rate.
 */

unsigned int hash_string(const char* str) {
  unsigned int hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;

  return hash;
}

/*
 * Returns a symbol for the given string. If this symbol is not
 * already in the symbol table, a new symbol will be created.
 */

Object* Symbol_get(const char* id) {
  unsigned int hash = hash_string(id);
  unsigned int pos = hash % symtab->bounds;

  // search for the key
  while (symtab->keys[pos]) {
    if (strcmp(symtab->keys[pos], id)) {
      pos = (pos+1) % symtab->bounds;
    }
    else return symtab->values[pos];
  }

  // key not found, create a new symbol
  pos = hash % symtab->bounds;

  while (symtab->keys[pos] && strcmp(symtab->keys[pos], id)) {
    pos = (pos+1) % symtab->bounds;
  }

  Object* symbol = Object_new(Symbol_proto);

  symtab->keys[pos] = id;
  symtab->values[pos] = symbol;
  symtab->size++;

  return symbol;
}

void Symbol_initProto() {
  Symbol_proto->data.ptr = symtab;
}
