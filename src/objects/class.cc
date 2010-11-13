#include "objects/objects.hh"
#include <cstdio>

namespace pyrite {

  Class::Class() {
    attributes = new Dictionary();
  }

  Object* Class::get_attribute(Object* key) {
    
    return attributes->get(key);;
  }

  Object* Class::set_attribute(Object* key, Object* value) {
    return attributes->set(key, value);
  }

}