#ifndef PROTY_OPCODE_H
#define PROTY_OPCODE_H

typedef enum OpCode {
    // general instructions
    OP_HLT,
    OP_NOP,
    OP_MOV,
    OP_RET,
    OP_JMP,
    OP_JNS,

    // primitives
    OP_SYM,
    OP_INT,
    OP_FLOAT,
    OP_STR,
    OP_BOOL,
    OP_NIL,

    // object methods
    OP_SET,
    OP_GET,
    OP_CALL,
    OP_SEND,

    // stack operations
    OP_PUSH,
    OP_POP,
} OpCode;

const char* OpCode_name(OpCode op);
int OpCode_size(OpCode op);

#endif
