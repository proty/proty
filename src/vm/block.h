#ifndef PROTY_BLOCK_H
#define PROTY_BLOCK_H

#include <stdio.h>
#include "opcodes.h"
#include "const.h"

typedef struct Block {
    int* data;
    size_t size;

    Const** consts;
    size_t constc;
} Block;

Block* Block_new();
void Block_delete(Block* self);

int Block_append(Block* self, OpCode op, ...);
void Block_replace(Block* self, int pos, int data);
int Block_position(Block* self);
int Block_const(Block* self, ConstType type, void* data);

void Block_write(Block* self, FILE* file);
Block* Block_read(FILE* file);
void Block_dump(Block* self);

#endif
