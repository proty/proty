#ifndef PYRITE_MODEL_IMPORT_HH
#define PYRITE_MODEL_IMPORT_HH

namespace pyrite {

  class ImportModel : public ExprModel {
    private:
      std::string name;

    public:
      ImportModel(std::string name)
        : name(name) { }

      Value* codegen(Compiler*);
  };

}

#endif
