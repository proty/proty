#include <cstdio>
#include <objects.hh>

using namespace proty;

Object* io_print(Object* obj) {
  printf("%s", (const char*)obj);
  return Bool::False;
}

extern "C" Object* io_init() {
  Object* io = new Object();

  Function* write = new Function((FunctionPtr*)io_print, 1);
  io->setProperty(new String("write"), write);

  return io;
}