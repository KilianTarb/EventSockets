#include "EventSocket.h"
#include "arpa/inet.h"
#include "gtest/gtest.h"

TEST (EventSocketTest, CreateSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    EXPECT_NE(sock.GetSocketFileDesciptor(), NULL);
}

TEST (EventSocketTest, BindSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8765);
    int convertion_err = inet_aton("127.0.0.1", &addr.sin_addr);
    EXPECT_EQ(convertion_err, 1);

    int bind_err = sock.Bind((struct sockaddr *)&addr, sizeof(addr));
    EXPECT_EQ(bind_err, 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
