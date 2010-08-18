#include "model/models.hh"

namespace pyrite {

  const Type* TypeModel::get(Compiler* c) {
    if (name == "#str") return Type::getInt8PtrTy(getGlobalContext());
    else if (name == "#int") return Type::getInt32Ty(getGlobalContext());
    else if (name == "#bool") return Type::getInt1Ty(getGlobalContext());
    else if (name == "#double") return Type::getDoubleTy(getGlobalContext());
    else if (name == "#void") return Type::getVoidTy(getGlobalContext());

    const Type* t = c->module->getTypeByName(name);

    if (!t) c->error("Type '" + name + "' not found.");

    if (t->isStructTy() || t->isOpaqueTy()) return PointerType::get(t, 0);
    else return t;
  }

}
