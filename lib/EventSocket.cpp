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

/**
 * @brief Invokes a Callback.
 *
 * @param callback
 * The function pointer
 * @return true
 * @return false
 */
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
 * @return 0 on success, errno on failed.
 */
int EventSocket::Bind(const sockaddr *addr, socklen_t len) {
    if (int err = bind(_socket_file_descriptor, addr, len) == 0) {
        _invokeCallback(_onBindReceiver);
        return 0;
    } else {
        _invokeCallback(_onBindFailed);
        return err;
    }
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
 * @return 0 on success, errno on failed.
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
 * @return True on success, False on error
 */
bool EventSocket::Listen(uint max_queue) {
    int err = listen(_socket_file_descriptor, max_queue);
    if (err == 0)
        _invokeCallback(_onListenReceiver);
    return err;
}

/**
 * @brief Accepts a connection in the listening queue.
 * 
 * @param remote_addr
 * sockaddr data structure to hold the remote endpoint.
 * 
 * @param len
 * length of the socket.
 * 
 * @return int. connecting socket's file descriptor, -1 on error
 */
int EventSocket::Accept(sockaddr *remote_addr, socklen_t *len) {
    _invokeCallback(_onAccpetingReceiver);
    int new_descriptor = accept(_socket_file_descriptor, remote_addr, len);

    if (new_descriptor != -1)
        _invokeCallback(_onAcceptReceiver);
    else
        _invokeCallback(_onAcceptFailedReceiver);

    return new_descriptor;
}

/**
 * @brief Connect to an endpoint. Invokes OnConnected and OnConnecting
 * 
 * @param addr 
 * Endpoint address
 * @param len 
 * Length of the endpoint address
 * @return True on success, False on error
 */
bool EventSocket::Connect(sockaddr *addr, socklen_t len) {
    _invokeCallback(_onConnectingReceiver);
    if (connect(_socket_file_descriptor, addr, len) == 0) {
        _invokeCallback(_onConnectedReceiver);
        return true;
    } else {
        _invokeCallback(_onConnectFailed);
        return false;
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
 * @return True on success, False on error
 */
bool EventSocket::Connect(const char *ip_cp, uint port, uint sock_type = AF_INET) {
    sockaddr_in remote_addr = _convertToINET(ip_cp, port);
    return Connect((sockaddr *)&remote_addr, sizeof(remote_addr));
}

/**
 * @brief Closes the socket's file desciptor. Invokes OnDisconnected and OnDisconnecting
 * 
 * @return True on success, False on error
 */
bool EventSocket::Disconnect() {
    _invokeCallback(_onDisconnectingReceiver);
    if (shutdown(_socket_file_descriptor, SHUT_RDWR) == 0) {
        _invokeCallback(_onDisconnectedReceiver);
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Sends BUF to the connected endpoint. The socket already be connect for this. @see Connect
 *
 * @param buf 
 * The buffer to send
 * @param len
 * Amount of bytes to sned
 * @param
 * Packet flags
 *
 * @return int. -1 on error, the amount of bytes send on success
 */
int EventSocket::Send(const void *buf, size_t len, int flags) {
    _invokeCallback(_onSendingReceiver);
    size_t bytes_sent = send(_socket_file_descriptor, buf, len, flags);
    if (bytes_sent == -1)
        return bytes_sent;
    _invokeCallback(_onSendReceiver);
    return bytes_sent;
}

/**
 * @brief Sends the buffer to the remote endpoint
 *
 * @param buf
 * The buffer
 * @param flags
 * Sending flags
 * @param len
 * Amount of bytes to send
 * @param remote
 * The remote endpoint
 *
 * @return bytes send
 */
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

/**
 * @brief Begin receiving data from the connected endpoint.
 *
 * @param buf
 * Buffer to hold the data
 * @param len
 * Amount of data to hold
 * @param flags
 * @return int. -1 on error, bytes received on success.
 */
int EventSocket::Receive(int remote_socket_descriptor, void *buf, size_t len, int flags) {
    _invokeCallback(_onReceivingReceiver);
    size_t bytes_received = recv(remote_socket_descriptor, buf, len, flags);
    if (bytes_received == -1)
        _invokeCallback(_onReceiveFailedReceiver);
    else
        _invokeCallback(_onReceiveReceiver);
    return bytes_received;
}

/**
 * @brief Receive from from remote address
 */
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
 * @brief Invokes the receiver when the socket bind attempt failed.
 */
void EventSocket::SubscribeOnBindFailed(Callback receiver) {
    _onBindFailed = receiver;
}

/**
 * @brief Invokes the receiver when the socket has connected to a remote endpoint.
 */
void EventSocket::SubscribeOnConnected(Callback receiver) {
    _onConnectedReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the connection attempt fails.
 */
void EventSocket::SubscribeOnConnectFailed(Callback receiver) {
    _onConnectFailed = receiver;
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
 * @brief Invokes the receiver when the socket has been put in a listening state.
 */
void EventSocket::SubscribeOnAccepting(Callback receiver) {
    _onAccpetingReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket has been put in a listening state.
 */
void EventSocket::SubscribeOnAccept(Callback receiver) {
    _onAcceptReceiver = receiver;
}

/**
 * @brief Invokes the receiver when the socket has been put in a listening state.
 */
void EventSocket::SubscribeOnAcceptFailed(Callback receiver) {
    _onAcceptFailedReceiver = receiver;
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

/**
 * @brief Invokes the receiver when the socket has received data.
 */
void EventSocket::SubscribeOnReceiveFailed(Callback receiver) {
    _onReceiveFailedReceiver = receiver;
}