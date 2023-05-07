#pragma once
/***
 * A mock server that mimics cqg WebAPI
 */

#ifdef USE_SSL
#include <websocketpp/config/asio.hpp>
#else
#endif

#include <websocketpp/server.hpp>

#include <iostream>
#include <cstdint>
#include <thread>

#ifdef USE_SSL
typedef websocketpp::server<websocketpp::config::asio_tls> wsserver;
#endif

class mock_server
{
    public:
    mock_server(int32_t port = 1234);
    ~mock_server();
    void run_loop();

    public:
    bool running = false;

    private:
    wsserver server;
    std::thread listener_thread;
};
