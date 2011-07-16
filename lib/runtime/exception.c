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

typedef int _Unwind_Action;
const _Unwind_Action _UA_SEARCH_PHASE = 1;
const _Unwind_Action _UA_CLEANUP_PHASE = 2;
const _Unwind_Action _UA_HANDLER_FRAME = 4;
const _Unwind_Action _UA_FORCE_UNWIND = 8;

struct _Unwind_Exception;
struct _Unwind_Context;

typedef void (*_Unwind_Exception_Cleanup_Fn) (_Unwind_Reason_Code reason,
                                              struct _Unwind_Exception *exc);

struct _Unwind_Exception {
  unsigned long exception_class;
  _Unwind_Exception_Cleanup_Fn exception_cleanup;
  unsigned long private_1;
  unsigned long private_2;
} __attribute__((__aligned__));

extern _Unwind_Reason_Code _Unwind_RaiseException(struct _Unwind_Exception*);

_Unwind_Reason_Code proty_personality(int version, _Unwind_Action actions,
                                      unsigned long exception_class,
                                      struct _Unwind_Exception* excpetionObject,
                                      struct _Unwind_Context* context) {
  if (version != 1)
    return _URC_FATAL_PHASE1_ERROR;

  return _URC_HANDLER_FOUND;

  return _URC_CONTINUE_UNWIND;
}

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
