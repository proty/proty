#ifndef PYRITE_VM_TYPE_HH
#define PYRITE_VM_TYPE_HH

namespace proty {

  class Type : public Object {
  private:
    String* name;

  public:
    Type(String* name) : name(name) {};
  };

}

#endif
