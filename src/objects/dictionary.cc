#include "objects/objects.hh"

#include <iostream>

namespace pyrite {

  struct Bucket {
    Object* key;
    Object* value;
  };

  unsigned int hash(Object* obj) {
    char* value = ((String*)obj)->get_value();

    int i;
    int h = 0;
    for (i = 0; value[i] != '\0'; i++) {
        h = 31*h + value[i];
    }

    return h;
  }

  Dictionary::Dictionary() {
    size = 0;
    bounds = 8;
    content = new Bucket*[bounds];
    
    memset(content, 0, bounds*sizeof(Bucket*));
  }

  Object* Dictionary::get(Object* key) {
    unsigned int h = hash(key) % bounds;
    std::cout << h << std::endl;
    while ((content[h] == 0) || (content[h]->key != key)) h = ((h+1) % bounds);
    return content[h]->value;
  }

  Object* Dictionary::set(Object* key, Object* value) {
    if (float(size)/float(bounds) > 0.75) {
      Bucket** tmp = new Bucket*[bounds*2];
      memset(tmp, 0, bounds*2*sizeof(Bucket*));

      for (int i = 0; i < bounds; i++) {
        if (content[i] != 0) {
          unsigned int h = hash(content[i]->key) %  bounds * 2;
          while (content[h] != 0 && content[h]->key != key) h = ((h+1) % bounds);
          tmp[h] = content[i];
        }
      }

      delete [] content;
      content = tmp;
      bounds *= 2;
    }

    unsigned int h = hash(key) % bounds;

    Bucket* b = new Bucket;
    b->key = key;
    b->value = value;

    while (content[h] != 0 && content[h]->key != key) h = ((h+1) % bounds);

    content[h] = b;
    size++;

    return 0;
  }

}