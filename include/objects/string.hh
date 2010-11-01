#ifndef PYRITE_OBJECTS_STRING_HH
#define PYRITE_OBJECTS_STRING_HH

namespace pyrite {

  class String : public Object {
  private:
    char* value;

  public:
    String(const char* value) : value((char*)value) {};

    Integer* length();

    operator char*() { return value; };
  };

}

#endif
