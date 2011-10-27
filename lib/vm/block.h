#ifndef PROTY_BLOCK_H
#define PROTY_BLOCK_H

#include "opcodes.h"

typedef struct Block {
    const char* name;
    int* data;
    int size;
} Block;

Block* Block_new(const char* name);
void Block_append(Block* self, OpCode op, int argc, ...);
void Block_dump(Block* self);

#endif
