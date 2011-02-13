#ifndef PROTY_VM_OBJECT_HH
#define PROTY_VM_OBJECT_HH

namespace proty {

  class HashMap;

  class Object {
  private:
    HashMap* slots;
    Object* proto;
    static Object* root;

  public:
    Object();
    Object(Object*);

    Object* createNew();
    static Object* createProto();

    Object* getProto();

    Object* getSlot(const char*);
    void setSlot(const char*, Object*);

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
