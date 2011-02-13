#include "objects.hh"
#include <cstdio>

namespace proty {

  Object* Integer::proto = Integer::createProto();

  Object* Integer::createProto() {
    return new Object();
  }

  Object* Integer::operator+(Object* b) {
    return new Integer(this->value + (int)*b);
  }

  Object* Integer::operator-(Object* b) {
    return new Integer(this->value - (int)*b);
  }

  Object* Integer::operator*(Object* b) {
    return new Integer(this->value * (int)*b);
  }

  Object* Integer::operator/(Object* b) {
    return new Integer(this->value / (int)*b);
  }

  Object* Integer::operator%(Object* b) {
    return new Integer(this->value % (int)*b);
  }

  Bool* Integer::operator==(Object* b) {
    return Bool::get(this->value == (int)*b);
  }

  Bool* Integer::operator!=(Object* b) {
    return Bool::get(this->value != (int)*b);
  }

  Bool* Integer::operator>(Object* b) {
    return Bool::get(this->value > (int)*b);
  }

  Bool* Integer::operator<(Object* b) {
    return Bool::get(this->value < (int)*b);
  }

  Bool* Integer::operator>=(Object* b) {
    return Bool::get(this->value >= (int)*b);
  }

  Bool* Integer::operator<=(Object* b) {
    return Bool::get(this->value <= (int)*b);
  }

  Integer::operator int() {
    return value;
  }

  Integer::operator const char*() {
    char* buffer = new char[50];
    sprintf(buffer, "%d", value);
    return (const char*)buffer;
  }

}
