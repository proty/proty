#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "module.h"

#define MAGIC 0x23425250

Module* Module_new() {
    Module* module = malloc(sizeof(Module));

    module->blocks = malloc(sizeof(Block*));
    module->blockc = 0;

    return module;
}

void Module_delete(Module* self) {
    for (int i = 0; i < self->blockc; i++) {
        free(self->blocks[i]);
    }
    free(self->blocks);
    free(self);
}

int Module_addBlock(Module* self, Block* block) {
    self->blocks = realloc(self->blocks, sizeof(char*)*(self->blockc+1));
    self->blocks[self->blockc] = block;
    return self->blockc++;
}

void Module_write(Module* self, FILE* file) {
    // write magic
    int magic = MAGIC;
    fwrite(&magic, sizeof(int), 1, file);

    // write block count
    fwrite(&self->blockc, sizeof(int), 1, file);

    // write blocks
    for (int i = 0; i < self->blockc; i++) {
        Block_write(self->blocks[i], file);
    }
}

Module* Module_read(FILE* file) {
    Module* module = Module_new();

    // check magic
    int magic;
    fread(&magic, sizeof(int), 1, file);
    assert(magic == MAGIC);

    // read blocks
    int blockc;
    fread(&blockc, sizeof(int), 1, file);

    for (int i = 0; i < blockc; i++) {
        Block* block = Block_read(file);
        Module_addBlock(module, block);
    }

    return module;
}

int Module_probe(FILE* file) {
    int magic = 0;
    fread(&magic, sizeof(int), 1, file);
    rewind(file);
    return magic == MAGIC;
}

void Module_dump(Module* self) {
    printf("MODULE DUMP\n");
    for (int i = 0; i < self->blockc; i++) {
        printf("\n#### Block %i\n", i);
        Block_dump(self->blocks[i]);
    }
}
