#ifndef PROTY_VM_LIST_HH
#define PROTY_VM_LIST_HH

namespace proty {

  class List : public Object {
  private:
    static Object* proto;
    Object** content;

  public:
    List() : Object(createProto()) {};

    static Object* createProto();

    Object* operator[](int);
  };

}

#endif
