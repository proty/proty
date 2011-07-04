#include "runtime/runtime.h"
#include "socket.h"

Object* net_socket(Object* self) {
  Object* socket = Socket_new();
  return socket;
}

Object* net_init() {
  Socket_proto = Socket_createProto();

  Object* net = Object_new(Object_proto);

  Object_setSlot(net, "socket", FUNC(net_socket, 1));

  return net;
}
