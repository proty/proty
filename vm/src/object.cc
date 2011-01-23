#include "objects.hh"
#include <cstdio>

namespace proty {

  Object* Object::getProperty(Object* key) {
    return properties->get(key);
  }

  Object* Object::setProperty(Object* key, Object* value) {
    properties->set(key, value);
    return value;
  }

  Object* Object::operator+(Object*) { /* throw error */ return 0; }
  Object* Object::operator-(Object*) { /* throw error */ return 0; }
  Object* Object::operator*(Object*) { /* throw error */ return 0; }
  Object* Object::operator/(Object*) { /* throw error */ return 0; }
  Object* Object::operator%(Object*) { /* throw error */ return 0; }

  Bool* Object::operator==(Object* b) { return Bool::get(b == this); }
  Bool* Object::operator!=(Object* b) { return Bool::get(b != this); }
  Bool* Object::operator>(Object*) { return Bool::False; }
  Bool* Object::operator<(Object*) { return Bool::False; }
  Bool* Object::operator>=(Object*) { return Bool::False; }
  Bool* Object::operator<=(Object*) { return Bool::False; }

  Object* Object::operator()(int, Object*[]) { /* throw error */ return 0; }

  Object::operator int() { /* throw error */ return 0; }
  Object::operator bool() { return true; }
  Object::operator const char*() { return "<Object>"; };

}
