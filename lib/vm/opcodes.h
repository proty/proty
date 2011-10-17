#ifndef PROTY_OPCODE_H
#define PROTY_OPCODE_H

typedef enum OpCode {
    OP_NOP,
    OP_MOV,
    OP_SYM,
    OP_INT,
    OP_SET,
    OP_GET,
    OP_CALL,
    OP_SEND,
} OpCode;

#endif
