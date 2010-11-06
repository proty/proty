#include "objects/objects.hh"

namespace pyrite {

  Object* Integer::operator+(Object* b) {
    return new Integer(this->value + (int)*(Integer*)b);
  }

  Object* Integer::operator-(Object* b) {
    return new Integer(this->value - (int)*(Integer*)b);
  }

  Object* Integer::operator*(Object* b) {
    return new Integer(this->value * (int)*(Integer*)b);
  }

  Object* Integer::operator/(Object* b) {
    return new Integer(this->value / (int)*(Integer*)b);
  }

  Object* Integer::operator%(Object* b) {
    return new Integer(this->value % (int)*(Integer*)b);
  }

}
