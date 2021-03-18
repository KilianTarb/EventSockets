#include "EventSocket.h"

EventSocket::EventSocket(int domain, int type, int protocol) {
    _socket_file_descriptor = socket(domain, type, protocol);
    if (_socket_file_descriptor == -1) {
        std::cout << "Could not create socket." << std::endl;
    }
}

int EventSocket::GetSocketFileDesciptor() {
    return _socket_file_descriptor;
}

int EventSocket::Connect(sockaddr *addr, socklen_t len) {
    return connect(_socket_file_descriptor, addr, len);
}