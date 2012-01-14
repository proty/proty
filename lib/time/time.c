#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "runtime/runtime.h"

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

    Object* time = Object_new(Object_getSlot(self, SYM(Time)));
    time->data.ptr = now;

    return time;
}

Object* time_sleep(Object* self, Object* duration) {
    if (duration->proto == Integer_proto) {
        sleep(duration->data.i);
    }
    return Qnil;
}

Object* time_init() {
    Object* Time_proto = Object_new(Object_proto);

    Object_setSlot(Time_proto, SYM(str), FUNC(Time_str, 1));
    Object_setSlot(Time_proto, SYM(strftime), FUNC(Time_strftime, 2));

    Object* time = Object_new(Object_proto);
    Object_setSlot(time, SYM(Time), Time_proto);
    Object_setSlot(time, SYM(now), FUNC(time_now, 1));
    Object_setSlot(time, SYM(sleep), FUNC(time_sleep, 2));

    return time;
}
