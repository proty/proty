#ifndef PYRITE_OBJECTS_FUNCTION_HH
#define PYRITE_OBJECTS_FUNCTION_HH

namespace pyrite {

  typedef Object* (FunctionPtr)(...);

  class Function : public Object {
  private:
    FunctionPtr* ref;
    unsigned int argc;

  public:
    Function(FunctionPtr* ref, unsigned int argc) : ref(ref), argc(argc) {};
  };

}

#endif
