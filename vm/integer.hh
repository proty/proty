#ifndef PROTY_VM_INTEGER_HH
#define PROTY_VM_INTEGER_HH

namespace proty {

  class Integer : public Object {
  private:
    int value;

  public:
    Integer(int value) : value(value) {};

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
