#include "runtime.h"
#include <stdlib.h>
#include <stdio.h>

Object* Exception_raise(Object* self) {
    return Qnil;
}

void Exception_initProto() {
    Object_setSlot(Exception_proto, SYM(raise), FUNC(Exception_raise, 1));
}
