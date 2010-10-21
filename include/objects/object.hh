#ifndef PYRITE_OBJECTS_OBJECT_HH
#define PYRITE_OBJECTS_OBJECT_HH

namespace pyrite {

  class Object {
  private:
    Type* type;
    Dictionary* attributes;

  public:
    Bool* kind_of(Type*);
  };

}

#endif
