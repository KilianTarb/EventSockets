#include "stdio.h"
#include "iostream"
#include <vector>
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>
#include "arpa/inet.h"
#include <unistd.h>

typedef std::function<void()> Callback;

class EventSocket
{
    private:
        int _socket_file_descriptor;

        Callback _onConnectedReceiver;
        Callback _onConnectingReceiver;
        Callback _onConnectFailed;
        Callback _onDisconnectedReceiver;
        Callback _onDisconnectingReceiver;
        Callback _onBindReceiver;
        Callback _onBindFailed;
        Callback _onListenReceiver;
        Callback _onAccpetingReceiver;
        Callback _onAcceptReceiver;
        Callback _onAcceptFailedReceiver;
        Callback _onSendingReceiver;
        Callback _onSendReceiver;
        Callback _onReceivingReceiver;
        Callback _onReceiveReceiver;

        sockaddr_in _convertToINET(const char *ip_cp, uint port);
        bool _invokeCallback(Callback callback);
    public:
        EventSocket(int domain, int type, int protocol);
        ~EventSocket();

        int GetSocketFileDesciptor();
        int Bind(const sockaddr *addr, socklen_t len);
        int Bind(const char *ip_cp, uint port, uint sock_type);
        int Listen(uint max_queue);
        int Accept(sockaddr *remote_addr, socklen_t *len);
        int Connect(sockaddr *addr, socklen_t len);
        int Connect(const char *ip_cp, uint port, uint sock_type);
        int Disconnect();
        int Send(const void *buf, size_t len, int flags);
        int SendTo(const void *buf, int flags, size_t len, sockaddr *remote);
        int SendTo(const void *buf, int flags, size_t len, const char *remote_ip, uint port, uint sock_type);
        int ReceiveFrom(void *buf, int flags, size_t len, sockaddr *remote);
        int ReceiveFrom(void *buf, int flags, size_t len, const char *remote_ip, uint port, uint sock_type);

        void SubscribeOnConnected(Callback receiver);
        void SubscribeOnConnecting(Callback receiver);
        void SubscribeOnConnectFailed(Callback receiver);
        void SubscribeOnDisconnected(Callback receiver);
        void SubscribeOnDisconnecting(Callback receiver);

        void SubscribeOnBind(Callback receiver);
        void SubscribeOnBindFailed(Callback receiver);
        void SubscribeOnListen(Callback receiver);
        void SubscribeOnAccepting(Callback receiver);
        void SubscribeOnAccept(Callback receiver);
        void SubscribeOnAcceptFailed(Callback receiver);
        void SubscribeOnSending(Callback receiver);
        void SubscribeOnSend(Callback receiver);
        void SubscribeOnReceiving(Callback receiver);
        void SubscribeOnReceive(Callback receiver);
};
