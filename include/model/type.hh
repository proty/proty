#ifndef PYRITE_MODEL_TYPE_HH
#define PYRITE_MODEL_TYPE_HH

namespace pyrite {

  class TypeModel {
  private:
    std::string name;
  
  public:
    TypeModel(std::string name) : name(name) { }

    const Type* get(Compiler*);
  };

}

#endif