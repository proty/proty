#ifndef PROTY_VM_HASH_HH
#define PROTY_VM_HASH_HH

namespace proty {

  struct HashBucket;

  class Hash : public Object {
  private:
    HashBucket** content;
    unsigned int size;
    unsigned int bounds;

  public:
    Hash();

    Object* get(Object*);
    Object* set(Object*, Object*);
  };

}

#endif