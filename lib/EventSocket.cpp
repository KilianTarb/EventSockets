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
    _onConnectingReceiver();
    if (connect(_socket_file_descriptor, addr, len)) {
        _onConnectedReceiver();
        return true;
    } else {
        return false;
    }
}

void EventSocket::SubscribeOnConnected(std::function<void()> receiver) {
    _onConnectedReceiver = receiver;
}

void EventSocket::SubscribeOnConnecting(std::function<void()> receiver) {
    _onConnectingReceiver = receiver;
}