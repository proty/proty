#include "objects/objects.hh"

namespace pyrite {

  Bool* Bool::True = new Bool(true);
  Bool* Bool::False = new Bool(false);

  Bool* Bool::get(bool value) { return value ? Bool::True : Bool::False; }

}
