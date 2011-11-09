#ifndef PROTY_BLOCK_H
#define PROTY_BLOCK_H

#include "opcodes.h"

typedef enum ConstType {
    CONST_STR,
    CONST_SYM,
    CONST_FLOAT,
} ConstType;

typedef struct Const {
    ConstType type;
    void* data;
} Const;

typedef struct Block {
    int* data;
    int size;

    Const** consts;
    int constc;
} Block;

Block* Block_new();
void Block_append(Block* self, OpCode op, ...);
int Block_const(Block* self, ConstType type, void* data);
void Block_dump(Block* self);

#endif
