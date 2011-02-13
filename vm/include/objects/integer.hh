#ifndef PROTY_VM_INTEGER_HH
#define PROTY_VM_INTEGER_HH

namespace proty {

  class Integer : public Object {
  private:
    static Object* proto;
    int value;

  public:
    Integer(int value) : Object(proto),
			 value(value) {}

    static Object* createProto();

    Object* operator+(Object*);
    Object* operator-(Object*);
    Object* operator*(Object*);
    Object* operator/(Object*);
    Object* operator%(Object*);

    Bool* operator==(Object*);
    Bool* operator!=(Object*);
    Bool* operator>(Object*);
    Bool* operator<(Object*);
    Bool* operator>=(Object*);
    Bool* operator<=(Object*);

    operator int();
    operator const char*();
  };

}

#endif
