#ifndef PYRITE_OBJECTS_CLASS_HH
#define PYRITE_OBJECTS_CLASS_HH

namespace pyrite {

  class Class : public Object {
  public:
    Class() {};

    Instance* new_instance(...);
  };

}

#endif
