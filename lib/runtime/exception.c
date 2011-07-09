#include "runtime.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {
  _URC_NO_REASON = 0,
  _URC_FOREIGN_EXCEPTION_CAUGHT = 1,
  _URC_FATAL_PHASE2_ERROR = 2,
  _URC_FATAL_PHASE1_ERROR = 3,
  _URC_NORMAL_STOP = 4,
  _URC_END_OF_STACK = 5,
  _URC_HANDLER_FOUND = 6,
  _URC_INSTALL_CONTEXT = 7,
  _URC_CONTINUE_UNWIND = 8
} _Unwind_Reason_Code;

struct _Unwind_Exception;

typedef void (*_Unwind_Exception_Cleanup_Fn) (_Unwind_Reason_Code reason,
                                              struct _Unwind_Exception *exc);

struct _Unwind_Exception {
  unsigned long exception_class;
  _Unwind_Exception_Cleanup_Fn exception_cleanup;
  unsigned long private_1;
  unsigned long private_2;
} __attribute__((__aligned__));

extern _Unwind_Reason_Code _Unwind_RaiseException(struct _Unwind_Exception*);

Object* Exception_raise(Object* self) {
  struct _Unwind_Exception* exp = malloc(sizeof(struct _Unwind_Exception));
  _Unwind_Reason_Code ret = _Unwind_RaiseException(exp);
  if (ret == _URC_END_OF_STACK) {
    printf("unhandled exception occured\n");
    exit(1);
  }
  else {
    printf("exception handling failure\n");
    exit(1);
  }
  return Qnil;
}

Object* Exception_createProto() {
  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "raise", FUNC(Exception_raise, 1));

  return proto;
}
