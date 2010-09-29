#ifndef PYRITE_OBJECTS_BOOL_HH
#define PYRITE_OBJECTS_BOOL_HH

namespace pyrite {

  class Bool : Object {
  private:
    bool value;

  public:
    Bool(bool value) : value(value) {};
  };

}

#endif
