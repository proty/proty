#ifndef PYRITE_OBJECTS_LIST_HH
#define PYRITE_OBJECTS_LIST_HH

namespace pyrite {

  class List : public Object {
  private:
    Object** content;

  public:
    List() {};

    Object* operator[](int);
  };

}

#endif