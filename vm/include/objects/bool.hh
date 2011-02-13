#ifndef PROTY_VM_BOOL_HH
#define PROTY_VM_BOOL_HH

namespace proty {

  class Bool : public Object {
  private:
    static Object* proto;
    bool value;

  public:
    Bool(bool value) : Object(proto), value(value) {};

    static Object* createProto();

    static Bool* get(bool value);

    static Bool* True;
    static Bool* False;

    operator bool() { return value; }
  };

}

#endif
