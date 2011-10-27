#ifndef PROTY_OPCODE_H
#define PROTY_OPCODE_H

typedef enum OpCode {
    OP_NOP,
    OP_RET,
    OP_SYM,
    OP_INT,
    OP_SET,
    OP_GET,
    OP_CALL,
    OP_SEND,
    OP_POP,
} OpCode;

static const char* OpCodeNames[] = {
    "nop",
    "ret",
    "sym",
    "int",
    "set",
    "get",
    "call",
    "send",
    "pop",
};

static const int OpCodeSize[] = {
    0,
    0,
    1,
    1,
    0,
    0,
    1,
    1,
    0,
};

#endif
