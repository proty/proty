#ifndef PYRITE_OBJECTS_INTEGER_HH
#define PYRITE_OBJECTS_INTEGER_HH

namespace pyrite {

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
