#include "mock_server.h"
#include "WebsocketClient.h"

#include <gtest/gtest.h>
#include <thread>
#include <chrono>

class mock_client : public WebsocketClient
{
    virtual void on_message_receive(message_ptr in) {}
};

TEST(mock_server_tests, ctor)
{
    try {
        mock_server server{1235};
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        EXPECT_TRUE(server.running);

        mock_client client;
        std::cout << "client constructed\n";
        bool result = client.connect("wss://localhost:1235");
        std::cout << "client connected\n";
        EXPECT_TRUE(result);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "test over, destroying" << std::endl;
    } catch( ... ) {
        FAIL();
    }
}

