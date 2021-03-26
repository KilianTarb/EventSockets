#include "stdio.h"
#include "iostream"
#include <vector>
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>
#include "arpa/inet.h"

typedef std::function<void()> Callback;

class EventSocket
{
    private:
        int _socket_file_descriptor;

        Callback _onConnectedReceiver;
        Callback _onConnectingReceiver;
        Callback _onDisconnectedReceiver;
        Callback _onDisconnectingReceiver;
        Callback _onBindReceiver;
        Callback _onListenReceiver;
        Callback _onSendingReceiver;
        Callback _onSendReceiver;
    public:
        EventSocket(int domain, int type, int protocol);
        ~EventSocket();

        int GetSocketFileDesciptor();
        int Bind(const sockaddr *addr, socklen_t len);
        int Bind(const char *ip_cp, uint port, uint sock_type);
        int Listen(uint max_queue);
        int Connect(sockaddr *addr, socklen_t len);
        int Connect(const char *ip_cp, uint port, uint sock_type);
        int Disconnect();
        int SendTo(const void *buf, int flags, size_t len, sockaddr *remote);
        int SendTo(const void *buf, int flags, size_t len, const char *remote_ip, uint port, uint sock_type);
        
        void SubscribeOnConnected(Callback receiver);
        void SubscribeOnConnecting(Callback receiver);
        void SubscribeOnDisconnected(Callback receiver);
        void SubscribeOnDisconnecting(Callback receiver);

        void SubscribeOnBind(Callback receiver);
        void SubscribeOnListen(Callback receiver);
        void SubscribeOnSending(Callback receiver);
        void SubscribeOnSend(Callback receiver);
};
