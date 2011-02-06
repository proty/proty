#include "phash.hh"

#include <cstdio>
#include <cstring>

namespace proty {

  struct PHashBucket {
    const char* key;
    Object* value;
  };

  unsigned int inline hash(const char* value) {
    int i;
    int h = 0;
    for (i = 0; value[i] != '\0'; i++) {
        h = 31*h + value[i];
    }
    return h;
  }

  PHash::PHash() {
    size = 0;
    bounds = 8;
    content = new PHashBucket*[bounds];

    memset(content, 0, bounds*sizeof(PHashBucket*));
  }

  Object* PHash::get(const char* key) {
    unsigned int h = hash(key) % bounds;
    while ((content[h] == 0) || !strcmp(content[h]->key, key)) h = ((h+1) % bounds);
    return content[h]->value;
  }

  Object* PHash::set(const char* key, Object* value) {
    if (float(size)/float(bounds) > 0.75) {
      PHashBucket** tmp = new PHashBucket*[bounds*2];
      memset(tmp, 0, bounds*2*sizeof(PHashBucket*));

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

    PHashBucket* b = new PHashBucket;
    b->key = key;
    b->value = value;

    while (content[h] != 0 && content[h]->key != key) h = ((h+1) % bounds);

    content[h] = b;
    size++;

    return 0;
  }

}
