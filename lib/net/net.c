#include "runtime/runtime.h"
#include "socket.h"

MODFUNC(net_socket, Object* self) {
  Object* socket = Socket_new();
  return socket;
}

EXPORT(net_Socket);

void net_init() {
  Socket_proto = Socket_createProto();

  INITFUNC(net_socket, 1);
  INITOBJ(net_Socket, Socket_proto);
}
