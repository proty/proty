#ifndef PYRITE_OBJECTS_DICTIONARY_HH
#define PYRITE_OBJECTS_DICTIONARY_HH

namespace pyrite {

  class Bucket;

  class Dictionary : public Object {
  private:
    Bucket** content;
    unsigned int size;
    unsigned int bounds;

  public:
    Dictionary();

    Object* get(Object*);
    Object* set(Object*, Object*);
  };

}

#endif