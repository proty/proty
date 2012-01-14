#ifndef IO_IO_H
#define IO_IO_H

Object* io_write(Object* self, Object* obj);
Object* io_print(Object* self, Object* obj);
Object* io_open(Object* self, Object* file, Object* mode);

#endif
