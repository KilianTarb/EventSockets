#include "stdio.h"
#include "iostream"
#include <vector>
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>

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
        Callback _onSendReceiver;
    public:
        EventSocket(int domain, int type, int protocol);
        ~EventSocket();

        int GetSocketFileDesciptor();
        int Bind(const sockaddr *addr, socklen_t len);
        int Connect(sockaddr *addr, socklen_t len);
        int Disconnect();
        int SendBytes();
        
        void SubscribeOnConnected(Callback receiver);
        void SubscribeOnConnecting(Callback receiver);
        void SubscribeOnDisconnected(Callback receiver);
        void SubscribeOnDisconnecting(Callback receiver);

        void SubscribeOnBind(Callback receiver);
        void SubscribeOnSend(Callback receiver);
};
