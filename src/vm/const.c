#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "block.h"
#include "const.h"

void Const_write(Const* self, FILE* file) {
    // write type
    fwrite(&self->type, sizeof(ConstType), 1, file);

    // write constant data
    switch (self->type) {
    case CONST_SYM:
    case CONST_STR: {
        size_t length = strlen(self->data);
        fwrite(&length, sizeof(size_t), 1, file);
        fwrite(self->data, sizeof(char), length, file);
        break;
    }

    case CONST_FLOAT:
        fwrite(self->data, sizeof(double), 1, file);
        break;
    }
}

void Const_read(Block* block, FILE* file) {
    // read type
    ConstType type;
    fread(&type, sizeof(ConstType), 1, file);

    // read data
    switch (type) {
    case CONST_SYM:
    case CONST_STR: {
        size_t length;
        fread(&length, sizeof(size_t), 1, file);

        char* data = malloc(sizeof(char)*length);
        fread(data, sizeof(char), length, file);

        Block_const(block, type, data);
        break;
    }

    case CONST_FLOAT: {
        double* data = malloc(sizeof(double));
        fread(data, sizeof(double), 1, file);
        Block_const(block, CONST_FLOAT, data);
        break;
    }
    }
}

void Const_dump(Const* self) {
    switch (self->type) {
    case CONST_SYM:
        printf("SYMBOL %s\n", (char*)self->data);
        break;

    case CONST_STR:
        printf("STRING %s\n", (char*)self->data);
        break;

    case CONST_FLOAT:
        printf("FLOAT  %f\n", *((double*)self->data));
        break;
    }
}
