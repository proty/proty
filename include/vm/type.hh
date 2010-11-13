#ifndef PYRITE_OBJECTS_TYPE_HH
#define PYRITE_OBJECTS_TYPE_HH

namespace pyrite {

  class Type : public Object {
  private:
    String* name;

  public:
    Type(String* name) : name(name) {};
  };

}

#endif
