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
    std::cout << "searched: " << key << std::endl;
    std::cout << "hash: " << h << " key: " << content[h]->key << std::endl;
    while ((content[h] == 0) || (content[h]->key != key)) {
      h = ((h+1) % bounds);
      std::cout << "hash: " << h << std::endl;
      if (content[h]) std::cout << "key: " << content[h]->key << ((content[h] == 0) || (content[h]->key != key)) << std::endl;
    }
    std::cout << "hash: " << h << " key: " << content[h]->key << std::endl;
    return content[h]->value;
  }

  Object* Dictionary::set(Object* key, Object* value) {
    unsigned int h = hash(key) % bounds;

    Bucket* b = new Bucket;
    b->key = key;
    b->value = value;

    while (content[h] != 0 && content[h]->key != key) h = ((h+1) % bounds);

    content[h] = b;
    size++;

    if (float(size)/float(bounds) > 0.75) {
      int oldbounds = bounds;
      bounds *= 2;

      Bucket** tmp = new Bucket*[bounds];
      memset(tmp, 0, bounds*sizeof(Bucket*));

      for (int i = 0; i < oldbounds; i++) {
        if (content[i] != 0) {
          unsigned int h = hash(content[i]->key) %  bounds;
          tmp[h] = content[i];
        }
      }

      delete content;
      content = tmp;
    }

    return 0;
  }

}

int main() {
  pyrite::Dictionary* dict = new pyrite::Dictionary();
  
  pyrite::String* test1 = new pyrite::String("Hello");
  pyrite::String* test2 = new pyrite::String("World");
  pyrite::String* test3 = new pyrite::String("Foo");
  pyrite::String* test4 = new pyrite::String("Bar");
  pyrite::String* test5 = new pyrite::String("Test");
  pyrite::String* test6 = new pyrite::String("This");
  pyrite::String* test7 = new pyrite::String("Fucking");
  pyrite::String* test8 = new pyrite::String("Code");
  pyrite::String* test9 = new pyrite::String("Now");
 
  std::cout << dict->set(test1, test2) << std::endl;
  std::cout << dict->set(test2, test1) << std::endl;
  std::cout << dict->set(test3, test1) << std::endl;
  std::cout << dict->set(test4, test1) << std::endl;
  std::cout << dict->set(test5, test1) << std::endl;
  std::cout << dict->set(test6, test1) << std::endl;
  std::cout << dict->set(test7, test1) << std::endl;
  std::cout << dict->set(test8, test1) << std::endl;
  std::cout << dict->set(test9, test1) << std::endl;

  std::cout << ((pyrite::String*)dict->get(test1))->get_value() << std::endl;
}