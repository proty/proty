#include "objects/objects.hh"
using namespace pyrite;

extern "C" {
  
  Object* add(Object* a, Object* b) {
    return a->operator+(b);
  }

  Object* sub(Object* a, Object* b) {
    return a->operator-(b);
  }

}