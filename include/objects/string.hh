#ifndef PYRITE_OBJECTS_STRING_HH
#define PYRITE_OBJECTS_STRING_HH

namespace pyrite {
  
  class String : Object {
  private:
    char* value;

  public:
    String(char* value) : value(value) {};

    Integer* length();
  };

}

#endif
