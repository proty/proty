#include <cstdio>
#include <objects.hh>

using namespace proty;

Object* io_print(Object* obj) {
  printf("%s\n", (const char*)(*obj));
  return 0;
}

Object* io_write(Object* obj) {
  printf("%s", (const char*)(*obj));
  return 0;
}

extern "C" Object* io_init() {
  Object* io = new Object();

  Function* print = new Function((FunctionPtr*)io_print, 1);
  io->setSlot("print", print);

  Function* write = new Function((FunctionPtr*)io_write, 1);
  io->setSlot("write", write);

  return io;
}
