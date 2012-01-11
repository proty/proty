#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include "load.h"

Object* load(State* state, const char* name) {
    char* lib_name = malloc(sizeof(char) * (6 + strlen(name) +
                                            strlen(SHARED_LIB_PREFIX) +
                                            strlen(SHARED_LIB_SUFFIX)));

    strcpy(lib_name, SHARED_LIB_PREFIX);
    strcat(lib_name,"proty-");
    strcat(lib_name, name);
    strcat(lib_name, SHARED_LIB_SUFFIX);

    void* lib = dlopen(lib_name, RTLD_LAZY);
    if (!lib) return Qnil;

    char* init_name = malloc(sizeof(char) * (strlen(name) + 5));
    strcpy(init_name, name);
    strcat(init_name, "_init");

    Object* (*init_func)();
    init_func = dlsym(lib, init_name);

    free(lib_name);
    free(init_name);

    return init_func ? init_func() : Qnil;
}
