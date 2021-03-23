#include "EventSocket.h"
#include "gtest/gtest.h"

GTEST_TEST (EventSocketTest, CreateSocket) {
    EventSocket sock(AF_INET, SOCK_STREAM, 0);
    EXPECT_NE(sock.GetSocketFileDesciptor(), NULL);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
