#include "EventSocket.h"

EventSocket::EventSocket(int domain, int type, int protocol) {
    _socket_file_descriptor = socket(domain, type, protocol);
    if (_socket_file_descriptor == -1) {
        std::cout << "Could not create socket." << std::endl;
    }
}

EventSocket::~EventSocket() {}

/**
 * @brief Converts and IPv4 address and port into binary.
 * 
 * @param ip_cp
 * IPv4 address and presentation format
 * @param port
 * Endpoint port
 *
 * @return sockaddr_in data structure
 */
sockaddr_in EventSocket::_convertToINET(const char *ip_cp, uint port) {
    sockaddr_in inet_addr;
    inet_addr.sin_family = AF_INET;
    inet_addr.sin_port = htons(port);
    inet_aton(ip_cp, &inet_addr.sin_addr);
    return inet_addr;
}

bool EventSocket::_invokeCallback(Callback callback) {
    if (callback == NULL)
        return false;
    callback();
    return true;
}

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
    _invokeCallback(_onBindReceiver);
    return err;
}

/**
 * @brief Bind socket to local endpoint
 *
 * @param ip_cp
 * IP address in presentation format.
 * @param port
 * Local port to bind to.
 * @param sock_type
 * Type of socket. Default is AF_INET.
 *
 * @return int. -1 if error, 0 if success
 */
int EventSocket::Bind(const char *ip_cp, uint port, uint sock_type = AF_INET) {
    sockaddr_in remote_addr = _convertToINET(ip_cp, port);
    return Bind((sockaddr *)&remote_addr, sizeof(remote_addr));
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
    if (err == 0)
        _invokeCallback(_onListenReceiver);
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
    _invokeCallback(_onConnectingReceiver);
    if (connect(_socket_file_descriptor, addr, len) == 0) {
        _invokeCallback(_onConnectedReceiver);
        return 0;
    } else {
        return -1;
    }
}

/**
 * @brief Connect to an endpoint. Invokes OnConnected and OnConnecting
 *
 * @param ip_cp
 * Address in presentation format
 * @param port
 * Remote port
 * @param sock_type
 * Socket type. Default is AF_INET
 *
 * @return int. -1 on err, 0 on success
 */
int EventSocket::Connect(const char *ip_cp, uint port, uint sock_type = AF_INET) {
    sockaddr_in remote_addr = _convertToINET(ip_cp, port);
    return Connect((sockaddr *)&remote_addr, sizeof(remote_addr));
}

/**
 * @brief Closes the socket's file desciptor. Invokes OnDisconnected and OnDisconnecting
 * 
 * @return int 
 */
int EventSocket::Disconnect() {
    _invokeCallback(_onDisconnectingReceiver);
    if (shutdown(_socket_file_descriptor, SHUT_RDWR) == 0) {
        _invokeCallback(_onDisconnectedReceiver);
        return true;
    } else {
        return false;
    }
}


int EventSocket::SendTo(const void *buf, int flags, size_t len, sockaddr *remote) {
    _invokeCallback(_onSendingReceiver);
    size_t sent_len = sendto(_socket_file_descriptor, buf, len, flags, remote, sizeof(remote));
    if (sent_len > 0)
        _invokeCallback(_onSendReceiver);
    return sent_len;
}

int EventSocket::SendTo(const void *buf, int flags, size_t len, const char *remote_ip, uint port, uint sock_type) {
    sockaddr_in remote_addr = _convertToINET(remote_ip, port);
    return SendTo(buf, flags, len, (struct sockaddr *)&remote_addr);
}

int EventSocket::ReceiveFrom(void *buf, int flags, size_t len, sockaddr *remote) {
    _invokeCallback(_onReceivingReceiver);
    size_t rlen = recvfrom(_socket_file_descriptor, buf, len, flags, remote, (socklen_t *)sizeof(remote));
    _invokeCallback(_onReceiveReceiver);
    return len;
}

int EventSocket::ReceiveFrom(void *buf, int flags, size_t len, const char *remote_ip, uint port, uint sock_type) {
    sockaddr_in remote_addr = _convertToINET(remote_ip, port);
    return ReceiveFrom(buf, flags, len, (struct sockaddr *)&remote_addr);
}

/**
 * @brief Invokes the receiver when the socket has binded.
 */
void EventSocket::SubscribeOnBind(Callback receiver) {
    _onBindReceiver = receiver;
}

/**
 * @brief Invokes the receiever when the socket has connected to a remote endpoint.
 */
void EventSocket::SubscribeOnConnected(Callback receiver) {
    _onConnectedReceiver = receiver;
}

/**
 * @brief Invokes the receiver before the socket attempts to connect to a remote endpoint.
 */
void EventSocket::SubscribeOnConnecting(Callback receiver) {
    _onConnectingReceiver = receiver;
}

/**
 * @brief Invokes the receiver before the socket attempts to disconnect.
 */
void EventSocket::SubscribeOnDisconnecting(Callback receiver) {
    _onDisconnectingReceiver = receiver;
}

/**
 * @brief Invokes thes receiver when the socket has disconnected.
 */
void EventSocket::SubscribeOnDisconnected(Callback receiver) {
    _onDisconnectedReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket is about to send data.
 */
void EventSocket::SubscribeOnSending(Callback receiver) {
    _onSendingReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket has sent data.
 */
void EventSocket::SubscribeOnSend(Callback receiver) {
    _onSendReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket has been put in a listening state.
 */
void EventSocket::SubscribeOnListen(Callback receiver) {
    _onListenReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket is about to receive data.
 */
void EventSocket::SubscribeOnReceiving(Callback receiver) {
    _onReceivingReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket has received data.
 */
void EventSocket::SubscribeOnReceive(Callback receiver) {
    _onReceiveReceiver = receiver;
}