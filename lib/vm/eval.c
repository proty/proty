#include <stdlib.h>
#include <stdio.h>
#include "eval.h"

#define POP() *(sp--)
#define TOP() *sp
#define PUSH(obj) *(++sp) = obj

Object* eval(Module* module) {
    int* pc = module->blocks[0]->data;

    Object* stack[256];
    Object** sp = stack;

    runtime_init();

    for (;;) {
        switch (*(pc++)) {
        case OP_INT:
            PUSH(Integer_new(*pc));
            pc++;
            break;

        case OP_SYM:
            PUSH(Symbol_get(module->symbols[*pc]));
            pc++;
            break;

        case OP_SEND: {
            Object* msg = POP();
            Object* obj = POP();
            int argc = POP();
            PUSH(Object_send(obj, msg, argc, sp-argc));
            pc++;
            break;
        }

        case OP_RET:
            return TOP();

        default:
            fprintf(stderr, "error. unknown instruction.");
            abort();
        }

    }
}
