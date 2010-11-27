#ifndef PYRITE_VM_BOOL_HH
#define PYRITE_VM_BOOL_HH

namespace pyrite {

  class Bool : public Object {
  private:
    bool value;

  public:
    Bool(bool value) : value(value) {};
    static Bool* get(bool value);

    static Bool* True;
    static Bool* False;

    operator bool() { return value; }
  };

}

#endif
