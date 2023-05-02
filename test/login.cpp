#include <gtest/gtest.h>

#include "CQGSession.h"
#include "mock_server.h"

#include <chrono>
#include <thread>

TEST(login_tests, DISABLED_login)
{
    // start web service
    mock_server server(1234);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    CQGSession session;
    bool result = session.connect("ws://localhost:1234");
    EXPECT_TRUE(result);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    result = session.logon("good_user", "good_password");
    EXPECT_TRUE(result);
    // TODO: check to make sure we are logged in. We should have gotten a reply
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
