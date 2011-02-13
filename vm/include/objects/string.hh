#ifndef PROTY_VM_STRING_HH
#define PROTY_VM_STRING_HH

namespace proty {

  class String : public Object {
  private:
    static Object* proto;
    const char* value;

  public:
    String(const char* value) : Object(proto),
				value(value) {}

    static Object* createProto();

    Integer* length();

    Object* operator+(Object*);

    Bool* operator==(Object*);
    Bool* operator!=(Object*);

    operator const char*() { return value; };
  };

}

#endif
