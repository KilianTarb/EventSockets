#include "stdio.h"
#include "iostream"
#include <vector>
#include <functional>
#include <sys/socket.h>
#include <sys/types.h>

class EventSocket
{
    private:
        int _socket_file_descriptor;

        std::function<void()> _onConnectedReceiver;
        std::function<void()> _onConnectingReceiver;
    public:
        EventSocket(int domain, int type, int protocol);
        ~EventSocket();

        int GetSocketFileDesciptor();
        int Connect(sockaddr *addr, socklen_t len);
        int SendBytes();
        
        void SubscribeOnConnected(std::function<void()> receiver);
        void SubscribeOnConnecting(std::function<void()> receiver);
};