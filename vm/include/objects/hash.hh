#ifndef PROTY_VM_HASH_HH
#define PROTY_VM_HASH_HH

namespace proty {

  class Hash : public Object {
  private:
    static Object* proto;

  public:
    Hash() : Object(proto) {}

    static Object* createProto();

    Object* get(Object*);
    Object* set(Object*, Object*);
  };

}

#endif
