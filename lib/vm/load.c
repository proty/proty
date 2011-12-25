#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"

Object* load(State* state, const char* name) {
    char* lib_name = malloc(sizeof(char) * (strlen("lib/lib") + strlen(name) + strlen(".so")));
    strcpy(lib_name,"lib/lib");
    strcat(lib_name, name);
    strcat(lib_name, ".so");

    void* lib = dlopen(lib_name, RTLD_LAZY);
    if (!lib) return Qnil;

    char* init_name = malloc(sizeof(char) * (strlen(name) + strlen("init_")));
    strcpy(init_name, name);
    strcat(init_name, "_init");

    Object* (*init_func)();
    init_func = dlsym(lib, init_name);

    free(lib_name);
    free(init_name);

    return init_func ? init_func() : Qnil;
}
