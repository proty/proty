#include "objects.hh"
#include "hashmap.hh"
#include <cstdio>

namespace proty {

  Object* Object::root = Object::createProto();

  Object::Object() {
    slots = new HashMap();
    this->proto = Object::root;
  }

  Object::Object(Object* proto) {
    slots = new HashMap();
    this->proto = proto;
  }

  Object* Object::createNew() {
    return new Object(this);
  }

  Object* Object::createProto() {
    return new Object(0);
  }

  Object* Object::getProto() {
    return proto;
  }

  Object* Object::getSlot(const char* key) {
    return slots->get(key);
  }

  void Object::setSlot(const char* key, Object* value) {
    slots->set(key, value);
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
