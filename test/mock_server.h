#pragma once
/***
 * A mock server that mimics cqg WebAPI
 */
#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <cstdint>
#include <thread>

class mock_server
{
    public:
    mock_server(int32_t port = 1234);
    ~mock_server();
    void run_loop();

    public:
    bool running = false;

    private:
    websocketpp::server<websocketpp::config::asio_tls> server;
    std::thread listener_thread;
};
