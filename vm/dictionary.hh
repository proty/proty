#ifndef PYRITE_VM_DICTIONARY_HH
#define PYRITE_VM_DICTIONARY_HH

namespace pyrite {

  struct DictBucket;

  class Dictionary : public Object {
  private:
    DictBucket** content;
    unsigned int size;
    unsigned int bounds;

  public:
    Dictionary();

    Object* get(Object*);
    Object* set(Object*, Object*);
  };

}

#endif