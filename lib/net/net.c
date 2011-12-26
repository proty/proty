#include "runtime/runtime.h"
#include "socket.h"

Object* net_socket(Object* self) {
    return Socket_new();
}

Object* net_init() {
    Socket_proto = Socket_createProto();

    Object* net = Object_new(Object_proto);
    Object_setSlot(net, SYM(socket), FUNC(net_socket, 1));
    Object_setSlot(net, SYM(Socket), Socket_proto);

    return net;
}
