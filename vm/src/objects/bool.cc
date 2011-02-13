#include "objects.hh"

namespace proty {

  Object* Bool::proto = Bool::createProto();

  Object* Bool::createProto() {
    return new Object();
  }

  Bool* Bool::True = new Bool(true);
  Bool* Bool::False = new Bool(false);

  Bool* Bool::get(bool value) { return value ? Bool::True : Bool::False; }

}
