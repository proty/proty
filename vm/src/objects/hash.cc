#include "objects.hh"

namespace proty {

  Object* Hash::proto = Hash::createProto();

  Object* Hash::createProto() {
    return new Object();
  }

  Object* Hash::get(Object* key) {
    return 0;
  }

  Object* Hash::set(Object* key, Object* value) {
    return 0;
  }

}
