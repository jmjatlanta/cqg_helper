#include "mock_server.h"
#include <thread>
#include <chrono>

int main(int argc, char** argv)
{
    mock_server server(1234);
    std::this_thread::sleep_for(std::chrono::seconds(60));

}

