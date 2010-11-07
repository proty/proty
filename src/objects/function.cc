#include "objects/objects.hh"
#include <cstdio>

namespace pyrite {

  Object* Function::operator()(unsigned int argc, Object** argv) {
    return ref(argv);
  }

}
