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

/**
 * @brief Set the socket to a listening states for accepting new connections
 *
 * @param max_queue
 * The amount of connections that can be in the queue
 *
 * @return int. -1 if error, 0 if success
 */
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
    if  (_onConnectingReceiver != NULL)
        _onConnectingReceiver();
    if (connect(_socket_file_descriptor, addr, len)) {
        if (_onConnectedReceiver != NULL)
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
    if (_onDisconnectingReceiver != NULL)
        _onDisconnectingReceiver();
    if (shutdown(_socket_file_descriptor, SHUT_RDWR)) {
        if (_onDisconnectedReceiver != NULL)
            _onDisconnectedReceiver();
        return true;
    } else {
        return false;
    }
}

void EventSocket::SubscribeOnConnected(Callback receiver) {
    _onConnectedReceiver = receiver;
}

void EventSocket::SubscribeOnConnecting(Callback receiver) {
    _onConnectingReceiver = receiver;
}

void EventSocket::SubscribeOnDisconnecting(Callback receiver) {
    _onDisconnectingReceiver = receiver;
}

void EventSocket::SubscribeOnDisconnected(Callback receiver) {
    _onDisconnectedReceiver = receiver;
}

void EventSocket::SubscribeOnListen(Callback receiver) {
    _onListenReceiver = receiver;
}
