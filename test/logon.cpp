#include <gtest/gtest.h>

#include "WebAPI/webapi_2.pb.h"
#include "CQGSession.h"
#include "mock_server.h"

#include <chrono>
#include <thread>

TEST(logon_tests, logon)
{
    // start web service
    //mock_server server(1234);
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    CQGSession session;
    std::cout << "logon_tests connecting\n";
    bool result = session.connect("ws://localhost:1234");
    EXPECT_TRUE(result);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    result = session.logon(1, "good_user", "good_password");
    std::cout << "logon_tests logon sent\n";
    EXPECT_TRUE(result);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    EXPECT_TRUE(session.is_logged_in());
    std::cout << "logon_tests test complete\n";
}
