#ifndef PYRITE_VM_FUNCTION_HH
#define PYRITE_VM_FUNCTION_HH

namespace proty {

  typedef Object* (FunctionPtr)(...);

  class Function : public Object {
  private:
    FunctionPtr* ref;
    unsigned int argc;

  public:
    Function(FunctionPtr* ref, unsigned int argc) : ref(ref), argc(argc) {};

    Object* operator()(int, Object*[]);
  };

}

#endif
