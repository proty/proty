#ifndef PYRITE_OBJECTS_INTEGER_HH
#define PYRITE_OBJECTS_INTEGER_HH

namespace pyrite {

  class Integer : public Object {
  private:
    int value;

  public:
    Integer(int value) : value(value) {};
    
    int get_value() { return value; };
  };

}

#endif
