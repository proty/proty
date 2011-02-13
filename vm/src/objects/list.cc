#include "objects.hh"

namespace proty {

  Object* List::proto = List::createProto();

  Object* List::createProto() {
    return new Object();
  }

}
