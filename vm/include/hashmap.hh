#ifndef PROTY_VM_HASHMAP_HH
#define PROTY_VM_HASHMAP_HH

namespace proty {

  class Object;
  struct HashMapBucket;

  class HashMap {
  private:
    HashMapBucket* content;
    unsigned int size;
    unsigned int bounds;

  public:
    HashMap();

    Object* get(const char*);
    Object* set(const char*, Object*);
  };

}

#endif
