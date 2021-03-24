#include "EventSocket.h"

EventSocket::EventSocket(int domain, int type, int protocol) {
    _socket_file_descriptor = socket(domain, type, protocol);
    if (_socket_file_descriptor == -1) {
        std::cout << "Could not create socket." << std::endl;
    }
}

EventSocket::~EventSocket() {}

/**
 * @brief Returns the file desciptor of the socket.
 * 
* @return int
 */
int EventSocket::GetSocketFileDesciptor() {
    return _socket_file_descriptor;
}

/**
 * @brief Bind socket to local endpoint.
 *
 * @param addr
 * Local endpoint address
 * @param len
 * Length of addr
 *
 * @return int. -1 if error, 0 if success
 */
int EventSocket::Bind(const sockaddr *addr, socklen_t len) {
    int err = bind(_socket_file_descriptor, addr, len);
    if (_onBindReceiver != NULL)
        _onBindReceiver();
    return err;
}

int EventSocket::Listen(uint max_queue) {
    int err = listen(_socket_file_descriptor, max_queue);
    if (_onListenReceiver != NULL && err == 0)
        _onListenReceiver();
    return err;
}

/**
 * @brief Connect to an endpoint. Invokes OnConnected and OnConnecting
 * 
 * @param addr 
 * Endpoint address
 * @param len 
 * Length of the endpoint address
 * @return int 
 */
int EventSocket::Connect(sockaddr *addr, socklen_t len) {
    _onConnectingReceiver();
    if (connect(_socket_file_descriptor, addr, len)) {
        _onConnectedReceiver();
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Closes the socket's file desciptor. Invokes OnDisconnected and OnDisconnecting
 * 
 * @return int 
 */
int EventSocket::Disconnect() {
    _onDisconnectingReceiver();
    if (shutdown(_socket_file_descriptor, SHUT_RDWR)) {
        _onDisconnectedReceiver();
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

void EventSocket::SubscribeOnDisconnecting(std::function<void()> receiver) {
    _onDisconnectingReceiver = receiver;
}

void EventSocket::SubscribeOnDisconnected(std::function<void()> receiver) {
    _onDisconnectedReceiver = receiver;
}

void EventSocket::SubscribeOnListen(std::function<void()> receiver) {
    _onListenReceiver = receiver;
}