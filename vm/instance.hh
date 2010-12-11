#ifndef PYRITE_VM_INSTANCE_HH
#define PYRITE_VM_INSTANCE_HH

namespace proty {

  class Instance : public Object {
  private:
    Class* base;

  public:
    Instance(Class* base) : base(base) {};
  };

}

#endif
