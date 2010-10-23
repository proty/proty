#ifndef PYRITE_OBJECTS_BOOL_HH
#define PYRITE_OBJECTS_BOOL_HH

namespace pyrite {

  class Bool : public Object {
  private:
    bool value;

  public:
    Bool(bool value) : value(value) {};
    static Bool* get(bool value);

    static Bool* True;
    static Bool* False;
  };

}

#endif
