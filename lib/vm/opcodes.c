#include "opcodes.h"

const char* OpCode_name(OpCode op) {
    switch (op) {
    case OP_HLT:   return "hlt";
    case OP_NOP:   return "nop";
    case OP_MOV:   return "mov";
    case OP_RET:   return "ret";
    case OP_JMP:   return "jmp";
    case OP_JNS:   return "jns";

    case OP_SYM:   return "sym";
    case OP_INT:   return "int";
    case OP_FLOAT: return "float";
    case OP_STR:   return "str";

    case OP_SET:   return "set";
    case OP_GET:   return "get";
    case OP_CALL:  return "call";
    case OP_SEND:  return "send";

    case OP_PUSH:  return "push";
    case OP_POP:   return "pop";
    default:       return "unknown";
    }
}

int OpCode_size(OpCode op) {
    switch (op) {
    case OP_HLT:   return 0;
    case OP_NOP:   return 0;
    case OP_MOV:   return 2;
    case OP_RET:   return 1;
    case OP_JMP:   return 1;
    case OP_JNS:   return 2;

    case OP_SYM:   return 2;
    case OP_INT:   return 2;
    case OP_FLOAT: return 2;
    case OP_STR:   return 2;

    case OP_SET:   return 4;
    case OP_GET:   return 3;
    case OP_CALL:  return 3;
    case OP_SEND:  return 4;

    case OP_PUSH:  return 1;
    case OP_POP:   return 1;
    default:       return 0;
    }
};
