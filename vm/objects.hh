#ifndef PYRITE_OBJECTS_OBJECTS_HH
#define PYRITE_OBJECTS_OBJECTS_HH

namespace pyrite {
  class Object;
  class Integer;
  class String;
  class Bool;
  class Type;
  class Function;
  class Class;
  class Instance;
  class Dictionary;
  class List;
}

#include "vm/object.hh"
#include "vm/integer.hh"
#include "vm/string.hh"
#include "vm/bool.hh"
#include "vm/type.hh"
#include "vm/function.hh"
#include "vm/instance.hh"
#include "vm/class.hh"
#include "vm/dictionary.hh"
#include "vm/list.hh"

#endif