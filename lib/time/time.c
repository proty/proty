#include "runtime/runtime.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

Object* Time_proto;

Object* Time_str(Object* self) {
  char* str = ctime(self->data.ptr);
  // remove linefeed
  str[strlen(str)-1] = '\0';
  return String_new(str);
}

Object* Time_strftime(Object* self, Object* format) {
  if (format->proto == String_proto) {
    struct tm* timeinfo;
    char buffer[80];

    timeinfo = localtime(self->data.ptr);
    strftime(buffer, 80, format->data.ptr, timeinfo);

    return String_new(buffer);
  }
  else {
    return Qnil;
  }
}

Object* time_now(Object* self) {
  time_t* now = malloc(sizeof(time_t));
  time(now);

  Object* time = Object_new(Time_proto);
  time->data.ptr = now;

  return time;
}

Object* time_init() {
  Time_proto = Object_new(Object_proto);

  Object_setSlot(Time_proto, "str", FUNC(Time_str, 1));
  Object_setSlot(Time_proto, "strftime", FUNC(Time_strftime, 2));

  Object* proto = Object_new(Object_proto);

  Object_setSlot(proto, "Time", Time_proto);
  Object_setSlot(proto, "now", FUNC(time_now, 1));

  return proto;
}
