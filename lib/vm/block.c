#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "block.h"

Block* Block_new(const char* name) {
    Block* block = malloc(sizeof(Block));
    block->data = malloc(sizeof(int));
    block->constants = malloc(sizeof(Constant));
    block->constantc = 0;
    block->size = 0;
    return block;
}

void Block_append(Block* self, OpCode op, ...) {
    va_list ap;
    int start = self->size++;
    int argc = OpCodeSize[op];

    self->size += argc;
    self->data = realloc(self->data, self->size*sizeof(int));

    self->data[start++] = op;

    va_start(ap, argc);
    for (int i = start; i < self->size; i++) {
        self->data[i] = va_arg(ap, int);
    }
    va_end(ap);
}

int Block_constant(Block* self, ConstantType type, void* data) {
    Constant* constant = malloc(sizeof(Constant));
    constant->type = type;
    constant->data = data;

    self->constants = realloc(self->constants, sizeof(Constant*)*(self->constantc+1));
    self->constants[self->constantc] = constant;
    return self->constantc++;
}

void Block_dump(Block* self) {
    for (int i = 0; i < self->size;) {
        OpCode op = self->data[i++];
        printf("%s\t", OpCodeNames[op]);
        for (int j = 0; j < OpCodeSize[op]; j++) {
            printf("%i ", self->data[i++]);
        }
        printf("\n");
    }
}
