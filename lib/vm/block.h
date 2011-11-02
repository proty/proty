#ifndef PROTY_BLOCK_H
#define PROTY_BLOCK_H

#include "opcodes.h"

typedef enum ConstantType {
    CONSTANT_STRING,
    CONSTANT_SYMBOL,
} ConstantType;

typedef struct Constant {
    ConstantType type;
    void* data;
} Constant;

typedef struct Block {
    int* data;
    int size;

    Constant** constants;
    int constantc;
} Block;

Block* Block_new();
void Block_append(Block* self, OpCode op, ...);
int Block_constant(Block* self, ConstantType type, void* data);
void Block_dump(Block* self);

#endif
