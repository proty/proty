#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#include "eval.h"

#define R(x) registers[x]
#define PCi *(pc++)
#define PC(x) (*(pc+x))

Object* eval(State* state, Block* block) {
    int* pc = block->data;
    int sp = state->sp;

    Object** registers = state->registers;
    Object** stack = state->stack;

    for (;;) {
        switch (PCi) {
        case OP_HLT:
            return Qnil;

        case OP_NOP:
            break;

        case OP_MOV:
            R(PC(0)) = R(PC(1));
            pc += 2;
            break;

        case OP_RET:
            return R(PCi);

        case OP_JMP:
            pc += PC(0);
            break;

        case OP_JNS:
            if (R(PC(1)) == Qfalse) {
                pc += PC(0);
            }
            else {
                pc += 2;
            }
            break;

        case OP_INT:
            R(PC(0)) = Integer_new(PC(1));
            pc += 2;
            break;

        case OP_SYM:
            R(PC(0)) = Symbol_get(block->consts[PC(1)]->data);
            pc += 2;
            break;

        case OP_STR:
            R(PC(0)) = String_new(block->consts[PC(1)]->data);
            pc += 2;
            break;

        case OP_FLOAT:
            R(PC(0)) = Float_new(*(double*)block->consts[PC(1)]->data);
            pc += 2;
            break;

        case OP_SEND: {
            int ret = PCi;
            Object* obj = R(PCi);
            Object* msg = R(PCi);
            int argc = PCi+1;

            Object** args = alloca(sizeof(Object*)*argc);
            args[0] = obj;
            for (int i = 1; i < argc; i++) {
                args[i] = stack[--sp];
            }

            R(ret) = Object_send(obj, msg, argc, args);
            break;
        }

        case OP_GET: {
            int ret = PCi;
            Object* obj = R(PCi);
            Object* slot = R(PCi);
            R(ret) = Object_getSlot(obj, slot);
            break;
        }

        case OP_SET: {
            int ret = PCi;
            Object* obj = R(PCi);
            Object* slot = R(PCi);
            Object* val = R(PCi);
            R(ret) = Object_setSlot(obj, slot, val);
            break;
        }

        case OP_PUSH:
            stack[sp++] = R(PCi);
            break;

        case OP_POP:
            R(PCi) = stack[--sp];
            break;

        default:
            fprintf(stderr, "error. unknown instruction.");
            abort();
        }
    }
}
