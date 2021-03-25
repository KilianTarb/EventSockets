#include "EventSocket.h"
#include "arpa/inet.h"
#include "gtest/gtest.h"


TEST (EventSocketTest, CreateListenerSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    int bind_err = sock.Bind("127.0.0.1", 8765, AF_INET);
    ASSERT_EQ(bind_err, 0);
    int listen_err = sock.Listen(10);
    ASSERT_EQ(listen_err, 0);
}

TEST (EventSocketTest, CreateClientSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    int bind_err = sock.Bind("127.0.0.1", 8766, AF_INET);
    ASSERT_EQ(bind_err, 0);
    int connect_err = sock.Connect("127.0.0.1", 8765, AF_INET);
    ASSERT_EQ(connect_err, 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
