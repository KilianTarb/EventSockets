#include "iostream"
#include <sys/socket.h>
#include <sys/types.h>

class EventSocket
{
    private:
        int _socket_file_descriptor;
    public:
        EventSocket(int domain, int type, int protocol);
        ~EventSocket();

        int GetSocketFileDesciptor();
        int Connect(sockaddr *addr, socklen_t len);
};