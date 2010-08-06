#include "model/models.hh"

namespace pyrite {

  const Type* TypeModel::get(Compiler* c) {
    const Type* t = c->module->getTypeByName(name);
    if (!t) c->error("Type '" + name + "' not found.");
    if (t->isStructTy() || t->isOpaqueTy()) {
      return PointerType::get(t, 0);
    }
    else {
      return t;
    }
  }

}
