#ifndef IO_FILE_H
#define IO_FILE_H

extern Object* File_proto;
Object* File_createProto();

Object* File_new(const char*, const char*);

#endif
