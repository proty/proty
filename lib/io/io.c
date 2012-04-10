#include <stdio.h>
#include <stdlib.h>
#include "runtime/runtime.h"
#include "file.h"

Object* io_write(Object* self, Object* obj) {
    if (obj->proto == String_proto) {
        printf("%s", (char*)obj->data.ptr);
    }
    else {
        Object* string = Object_send(obj, SYM(str), 0, 0);
        if (string != Qnil) {
            printf("%s", (char*)string->data.ptr);
        }
        else {
            printf("<%p>", obj);
        }
    }
    return Qnil;
}

Object* io_read(Object* self) {
    char* str = malloc(sizeof(char) * 50);
    int limit = 50;
    int size = 0;

    while (1) {
        char c = fgetc(stdin);
        if (c == '\n') break;

        str[size++] = c;

        if (size == limit) {
            limit += 50;
            str = realloc(str, sizeof(char) * limit);
        }
    }

    str[size++] = '\0';

    Object* ret = String_new(str);
    free(str);
    return ret;
}

Object* io_print(Object* self, Object* obj) {
    Object* ret = io_write(self, obj);
    printf("\n");
    return ret;
}

Object* io_open(Object* self, Object* file, Object* mode) {
    if (file->proto == String_proto && mode->proto == String_proto) {
        return File_new(file->data.ptr, mode->data.ptr);
    }
    else {
        return Qnil;
    }
}

Object* io_init() {
    File_proto = File_createProto();

    Object* io = Object_new(Object_proto);

    Object_setSlot(io, SYM(write), FUNC(io_write, 2));
    Object_setSlot(io, SYM(read), FUNC(io_read, 1));
    Object_setSlot(io, SYM(print), FUNC(io_print, 2));
    Object_setSlot(io, SYM(open), FUNC(io_open, 3));
    Object_setSlot(io, SYM(File), File_proto);

    return io;
}
