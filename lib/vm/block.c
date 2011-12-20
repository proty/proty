#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "block.h"

Block* Block_new(const char* name) {
    Block* block = malloc(sizeof(Block));
    block->data = malloc(sizeof(int));
    block->consts = malloc(sizeof(Const));
    block->constc = 0;
    block->size = 0;
    return block;
}

void Block_replace(Block* self, int pos, int data) {
    self->data[pos] = data;
}

int Block_append(Block* self, OpCode op, ...) {
    va_list ap;
    int start = self->size++;
    int argc = OpCode_size(op);

    self->size += argc;
    self->data = realloc(self->data, self->size*sizeof(int));

    self->data[start++] = op;

    va_start(ap, argc);
    for (int i = start; i < self->size; i++) {
        self->data[i] = va_arg(ap, int);
    }
    va_end(ap);

    return self->size - argc - 1;
}

int Block_position(Block* self) {
    return self->size;
}

int Block_const(Block* self, ConstType type, void* data) {
    Const* constant = malloc(sizeof(Const));
    constant->type = type;
    constant->data = data;

    self->consts = realloc(self->consts, sizeof(Const*)*(self->constc+1));
    self->consts[self->constc] = constant;
    return self->constc++;
}

void Block_dump(Block* self) {
    for (int i = 0; i < self->size;) {
        OpCode op = self->data[i++];
        printf("%04d %s\t", i, OpCode_name(op));
        for (int j = 0; j < OpCode_size(op); j++) {
            printf("%i ", self->data[i++]);
        }
        printf("\n");
    }
}
