#include "runtime/runtime.h"
#include "socket.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>

Object* Socket_proto;

Object* Socket_new() {
    return Object_new(Socket_proto);
}

Object* Socket_connect(Object* self, Object* host, Object* port) {
    int sockfd;
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host->data.ptr);
    addr.sin_port = htons(port->data.i);

    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    self->data.i = sockfd;

    return Qtrue;
}

Object* Socket_send(Object* self, Object* message) {
    int sockfd = self->data.i;
    char* data = message->data.ptr;

    send(sockfd, data, strlen(data), 0);

    return Qtrue;
}

Object* Socket_recv(Object* self, Object* length) {
    int sockfd = self->data.i;
    int len = length->data.i;

    char* buffer = malloc(len);

    int received = 0;
    while (received < len) {
        int bytes;
        if ((bytes = recv(sockfd, buffer, len, 0)) < 1) {
            break;
        }
        received += bytes;
    }
    buffer[received] = '\0';

    Object* str = String_new(buffer);
    free(buffer);

    return str;
}

Object* Socket_close(Object* self) {
    int sockfd = self->data.i;
    close(sockfd);
    return Qnil;
}

Object* Socket_createProto() {
    Object* proto = Object_new(Object_proto);

    Object_setSlot(proto, SYM(connect), FUNC(Socket_connect, 3));
    Object_setSlot(proto, SYM(send), FUNC(Socket_send, 2));
    Object_setSlot(proto, SYM(recv), FUNC(Socket_recv, 2));
    Object_setSlot(proto, SYM(close), FUNC(Socket_close, 1));

    return proto;
}
