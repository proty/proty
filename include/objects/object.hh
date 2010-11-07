#ifndef PYRITE_OBJECTS_OBJECT_HH
#define PYRITE_OBJECTS_OBJECT_HH

namespace pyrite {

  class Object {
  private:
    Type* type;
    Dictionary* attributes;

  public:
    Bool* kind_of(Type*);

    virtual Object* operator+(Object*);
    virtual Object* operator-(Object*);
    virtual Object* operator*(Object*);
    virtual Object* operator/(Object*);
    virtual Object* operator%(Object*);

    virtual Bool* operator==(Object*);
    virtual Bool* operator!=(Object*);
    virtual Bool* operator>(Object*);
    virtual Bool* operator<(Object*);
    virtual Bool* operator>=(Object*);
    virtual Bool* operator<=(Object*);

    virtual operator int();
    virtual operator bool();
    virtual operator char*();
  };

}

#endif
