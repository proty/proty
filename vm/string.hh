#ifndef PYRITE_VM_STRING_HH
#define PYRITE_VM_STRING_HH

namespace proty {

  class String : public Object {
  private:
    const char* value;

  public:
    String(const char* value) : value(value) {}

    Integer* length();

    Object* operator+(Object*);

    Bool* operator==(Object*);
    Bool* operator!=(Object*);

    operator const char*() { return value; };
  };

}

#endif
