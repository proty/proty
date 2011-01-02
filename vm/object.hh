#ifndef PROTY_VM_OBJECT_HH
#define PROTY_VM_OBJECT_HH

namespace proty {

  class Object {
  private:
    Type* type;

  public:
    Bool* kind_of(Type*);

    virtual Object* get_attribute(Object*);
    virtual Object* set_attribute(Object*, Object*);

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

    virtual Object* operator()(int, Object*[]);

    virtual operator int();
    virtual operator bool();
    virtual operator const char*();
  };

}

#endif
