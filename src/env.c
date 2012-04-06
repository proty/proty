#include "env.h"

const int* proty_version() {
    static const int version[] = {
        VERSION_MAJOR,
        VERSION_MINOR,
        VERSION_PATCH
    };
    return version;
}

const char* proty_version_string() {
    return VERSION;
}

const char* proty_platform() {
    return PLATFORM;
}

const char* proty_prefix() {
    return PREFIX;
}

const char* proty_build_type() {
    return BUILD_TYPE;
}

const char* proty_copyright() {
    return COPYRIGHT;
}
