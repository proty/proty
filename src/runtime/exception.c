#include <stdlib.h>
#include <stdio.h>
#include "runtime.h"
#include "vm/state.h"

Object* Exception_new(const char* str) {
    Object* excp = Object_new(Exception_proto);
    excp->data.ptr = (void*)str;
    return excp;
}

Object* Exception_raise(Object* self) {
    State* state = State_getGlobalState();
    state->exception = self;

    longjmp(state->excp_buf, 1);

    // never reached
    return Qnil;
}

void Exception_initProto() {
    Object_setSlot(Exception_proto, SYM(raise), FUNC(Exception_raise, 1));
}
