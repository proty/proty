#ifndef PYRITE_OBJECTS_FUNCTION_HH
#define PYRITE_OBJECTS_FUNCTION_HH

namespace pyrite {

  class Function : public Object {
  private:
    Object* (*ref)(...);

  public:
    Function(Object* (ref)(...)) : ref(ref) {};
  };

}

#endif
