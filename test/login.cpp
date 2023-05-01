#include <gtest/gtest.h>

#include "CQGSession.h"

TEST(login_tests, connect)
{
    CQGSession session;
    session.host_name = "wss://demoapi.cqg.com:443";
    session.user_name = "";
    session.password = "";
    bool result = session.connect();
    EXPECT_TRUE(result);
}
