#ifndef PYRITE_OBJECTS_CLASS_HH
#define PYRITE_OBJECTS_CLASS_HH

namespace pyrite {

  class Class : public Object {
  private:
    Dictionary* attributes;

  public:
    Class();

    Object* get_attribute(Object*);
    Object* set_attribute(Object*, Object*);

    Instance* new_instance(Object**);
  };

}

#endif
