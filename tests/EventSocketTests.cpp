#include "EventSocket.h"
#include "arpa/inet.h"
#include "gtest/gtest.h"


TEST (EventSocketTest, CreateListenerSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8766);

    int convertion_err = inet_aton("127.0.0.1", &addr.sin_addr);
    ASSERT_EQ(convertion_err, 1);

    int bind_err = sock.Bind((struct sockaddr *)&addr, sizeof(addr));
    ASSERT_EQ(bind_err, 0);

    int listen_err = sock.Listen(10);
    ASSERT_EQ(listen_err, 0);
}

TEST (EventSocketTest, CreateClientSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8767);

    int convertion_err = inet_aton("127.0.0.1", &addr.sin_addr);
    ASSERT_EQ(convertion_err, 1);

    int bind_err = sock.Bind((struct sockaddr *)&addr, sizeof(addr));
    ASSERT_EQ(bind_err, 0);

    struct sockaddr_in remote_addr;
    remote_addr.sin_family = AF_INET;
    addr.sin_port = htons(8766);
    int remote_convertion_err = inet_aton("127.0.0.1", &remote_addr.sin_addr);

    int connect_err = sock.Connect((struct sockaddr *)&remote_addr, sizeof(remote_addr));
    ASSERT_EQ(connect_err, 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
