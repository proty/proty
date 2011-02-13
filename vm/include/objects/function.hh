#ifndef PROTY_VM_FUNCTION_HH
#define PROTY_VM_FUNCTION_HH

namespace proty {

  typedef Object* (FunctionPtr)(...);

  class Function : public Object {
  private:
    static Object* proto;
    FunctionPtr* ref;
    unsigned int argc;

  public:
    Function(FunctionPtr* ref, unsigned int argc)
      : Object(proto), ref(ref), argc(argc) {};

    static Object* createProto();

    Object* operator()(int, Object*[]);
  };

}

#endif
