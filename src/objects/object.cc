#include "objects/objects.hh"

namespace pyrite {

  Bool* Object::operator==(Object* b) { return Bool::get(b == this); }

  Bool* Object::operator!=(Object* b) { return Bool::get(b != this); }

  Bool* Object::operator>(Object*) { return Bool::False; }

  Bool* Object::operator<(Object*) { return Bool::False; }

  Bool* Object::operator>=(Object*) { return Bool::False; }
  
  Bool* Object::operator<=(Object*) { return Bool::False; }

}
