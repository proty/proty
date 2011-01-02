#ifndef PROTY_VM_CLASS_HH
#define PROTY_VM_CLASS_HH

namespace proty {

  class Class : public Object {
  private:
    Hash* attributes;

  public:
    Class();

    Object* get_attribute(Object*);
    Object* set_attribute(Object*, Object*);

    Instance* new_instance(Object**);
  };

}

#endif
