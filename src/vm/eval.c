#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "module.h"
#include "eval.h"
#include "load.h"

#define R(x) registers[x]
#define PCi *(pc++)
#define PC(x) (*(pc+x))

Object* eval(State* state, int id, int argc) {
    Block* block = state->module->blocks[id];

    int* pc = block->data;
    int sp = state->sp;

    Object** stack = state->stack;
    Object** registers = stack+sp-argc;

    sp += block->regc - argc;

    int* catch = 0;
    if (setjmp(state->excp_buf)) {
        if (!catch) {
            printf("Unhandled Exception: %s\n", (const char*)state->exception->data.ptr);
            abort();
        }
        else {
            pc = catch;
        }
    }

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
            state->sp = sp - block->regc;
            return R(PCi);

        case OP_JMP:
            pc += PC(0);
            break;

        case OP_JNS:
            if (R(PC(0)) == Qfalse) {
                pc += PC(1);
            }
            else {
                pc += 2;
            }
            break;

        case OP_FUN:
            R(PC(0)) = Function_new(PC(1), PC(2));
            pc += 3;
            break;

        case OP_LOAD:
            R(PC(0)) = load(state, block->consts[PC(1)]->data);
            pc += 2;
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

        case OP_NIL:
            R(PCi) = Qnil;
            break;

        case OP_BOOL:
            R(PC(0)) = PC(1) ? Qtrue : Qfalse;
            pc += 2;
            break;

        case OP_LIST: {
            int ret = PCi;
            int argc = PCi;
            Object* list = List_new();

            for (int i = sp - argc; i < sp; i++) {
                List_append(list, stack[i]);
            }
            sp -= argc;

            R(ret) = list;
            break;
        }

        case OP_HASH: {
            int ret = PCi;
            int argc = PCi;
            Object* hash = Hash_new();

            for (int i = 0; i < argc; i++) {
                Object* obj = stack[--sp];
                Object* key = stack[--sp];
                Hash_set(hash, key, obj);
            }

            R(ret) = hash;
            break;
        }

        case OP_CALL: {
            int ret = PCi;
            Object* obj = R(PCi);
            int argc = PCi;

            state->sp = sp;

            if (obj->proto == Function_proto) {
                assert(argc == Function_getArgc(obj));
                R(ret) = eval(state, Function_getId(obj), argc);
            }
            else {
                R(ret) = Object_call(obj, argc, stack+sp-argc);
                sp -= argc;
            }

            break;
        }

        case OP_SEND: {
            int ret = PCi;
            Object* obj = R(PCi);
            Object* msg = R(PCi);
            int argc = PCi;

            state->sp = sp;

            if (obj->proto == Function_proto) {
                assert(argc == Function_getArgc(obj));
                R(ret) = eval(state, Function_getId(obj), argc);
            }
            else {
                R(ret) = Object_send(obj, msg, argc, stack+sp-argc);
                sp -= argc;
            }

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

        case OP_TRY:
            catch = pc + PC(0);
            pc++;
            break;

        case OP_ENDTRY:
            catch = 0;
            break;

        case OP_PUSH:
            stack[sp++] = R(PCi);
            break;

        case OP_POP:
            R(PCi) = stack[--sp];
            break;

        default:
            fprintf(stderr, "error: unknown instruction (%i)\n", *(pc-1));
            abort();
        }
    }
}
