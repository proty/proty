#ifndef PROTY_OPCODE_H
#define PROTY_OPCODE_H

typedef enum OpCode {
    OP_NOP,
    OP_RET,
    OP_SYM,
    OP_INT,
    OP_FLOAT,
    OP_STR,
    OP_SET,
    OP_GET,
    OP_CALL,
    OP_SEND,
    OP_PUSH,
    OP_POP,
} OpCode;

static const char* OpCodeNames[] = {
    "nop",
    "ret",
    "sym",
    "int",
    "float",
    "str",
    "set",
    "get",
    "call",
    "send",
    "push",
    "pop",
};

static const int OpCodeSize[] = {
    0,
    1,
    2,
    2,
    2,
    2,
    4,
    3,
    3,
    4,
    1,
    1,
};

#endif