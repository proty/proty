#ifndef PROTY_CONST_H
#define PROTY_CONST_H

struct Block;

typedef enum ConstType {
    CONST_STR,
    CONST_SYM,
    CONST_FLOAT,
} ConstType;

typedef struct Const {
    ConstType type;
    void* data;
} Const;

void Const_write(Const* self, FILE* file);
void Const_read(struct Block* block, FILE* file);
void Const_dump(Const* self);

#endif
