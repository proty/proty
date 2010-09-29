#ifndef PYRITE_OBJECTS_INTEGER_HH
#define PYRITE_OBJECTS_INTEGER_HH

namespace pyrite {

  class Integer : Object {
  private:
    int value;

  public:
    Integer(int value) : value(value) {};
  };

}

#endif
