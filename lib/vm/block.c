#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "block.h"

Block* Block_new(const char* name) {
    Block* block = malloc(sizeof(Block));
    block->data = malloc(sizeof(int));
    block->name = name;
    block->size = 0;
    return block;
}

void Block_append(Block* self, OpCode op, int argc, ...) {
    va_list ap;
    int start = self->size++;

    self->size += argc;
    self->data = realloc(self->data, self->size*sizeof(int));

    self->data[start++] = op;

    va_start(ap, argc);
    for (int i = start; i < self->size; i++) {
        self->data[i] = va_arg(ap, int);
    }
    va_end(ap);
}

void Block_dump(Block* self) {
    printf("%s:\n", self->name);
    for (int i = 0; i < self->size;) {
        OpCode op = self->data[i++];
        printf("  %s\t", OpCodeNames[op]);
        for (int j = 0; j < OpCodeSize[op]; j++) {
            printf("%i ", self->data[i++]);
        }
        printf("\n");
    }
}
