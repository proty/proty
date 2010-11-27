#include "objects.hh"
#include <cstdio>

namespace pyrite {

  Object* Function::operator()(int argc, Object* argv[]) {
    // @todo: find a better method to pass the arguments
    Object* ret;
    switch (argc) {
      case 0:  ret = ref(); break;
      case 1:  ret = ref(argv[0]); break;
      case 2:  ret = ref(argv[0], argv[1]); break;
      case 3:  ret = ref(argv[0], argv[1], argv[2]); break;
      case 4:  ret = ref(argv[0], argv[1], argv[2], argv[3]); break;
      case 5:  ret = ref(argv[0], argv[1], argv[2], argv[3], argv[4]); break;
      case 6:  ret = ref(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]); break;
      case 7:  ret = ref(argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]); break;
    }
    return ret;
  }

}
