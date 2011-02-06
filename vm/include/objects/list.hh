#ifndef PROTY_VM_LIST_HH
#define PROTY_VM_LIST_HH

namespace proty {

  class List : public Object {
  private:
    Object** content;

  public:
    List() {};

    Object* operator[](int);
  };

}

#endif