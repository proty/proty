#include "hashmap.hh"

#include <cstdio>
#include <cstring>

namespace proty {

  struct HashMapBucket {
    const char* key;
    Object* value;
  };

  unsigned int inline hash(const char* key) {
    int hash = 0;
    for (; *key; ++key) {
      hash += *key;
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
  }

  HashMap::HashMap() {
    size = 0;
    bounds = 8;
    content = new HashMapBucket[bounds];

    memset(content, 0, bounds*sizeof(HashMapBucket));
  }

  Object* HashMap::get(const char* key) {
    unsigned int h = hash(key) % bounds;
    while (content[h].key != 0) {
      if (strcmp(content[h].key, key)) {
	return content[h].value;
      }
      h = ((h+1) % bounds);
    }
    return 0;
  }

  Object* HashMap::set(const char* key, Object* value) {
    if (float(size)/float(bounds) > 0.75) {
      HashMapBucket* tmp = new HashMapBucket[bounds*2];
      memset(tmp, 0, bounds*2*sizeof(HashMapBucket));

      for (int i = 0; i < bounds; i++) {
        if (content[i].key != 0) {
          unsigned int h = hash(content[i].key) % bounds * 2;
          while (content[h].key != 0 && !strcmp(content[h].key, key)) {
	    h = ((h+1) % bounds);
	  }
          tmp[h] = content[i];
        }
      }

      delete [] content;
      content = tmp;
      bounds *= 2;
    }

    unsigned int h = hash(key) % bounds;
    while (content[h].key != 0 && content[h].key != key) {
      h = ((h+1) % bounds);
    }

    content[h].key = key;
    content[h].value = value;
    size++;

    return 0;
  }

}
