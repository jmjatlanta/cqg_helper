#include "mock_server.h"

#include <gtest/gtest.h>

TEST(mock_server_tests, ctor)
{
    try {
        mock_server s(1234);
    } catch( ... ) {
        FAIL();
    }
}

