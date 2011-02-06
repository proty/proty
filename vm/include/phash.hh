#ifndef PROTY_VM_PHASH_HH
#define PROTY_VM_PHASH_HH

namespace proty {

  class Object;
  struct PHashBucket;

  class PHash {
  private:
    PHashBucket** content;
    unsigned int size;
    unsigned int bounds;

  public:
    PHash();

    Object* get(const char*);
    Object* set(const char*, Object*);
  };

}

#endif
