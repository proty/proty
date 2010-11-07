#include "objects/objects.hh"
#include <cstring>
#include <cstdio>

namespace pyrite {

  Integer* String::length() {
    return new Integer(strlen(value));
  }

  Object* String::operator+(Object* b) {
    const char* other = (const char*)*b;
    char* all = new char[strlen(value)+strlen(other)];
    strcpy(all, value);
    strcat(all, other);
    return new String((const char*)all);
  }

  Bool* String::operator==(Object* b) {
    return Bool::get(strcmp(value, (const char*)*b) == 0);
  }

  Bool* String::operator!=(Object* b) {
    return Bool::get(strcmp(value, (const char*)*b) != 0);
  }

}
