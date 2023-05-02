#pragma once
/***
 * A mock server that mimics cqg WebAPI
 */
#include <websocketpp/config/asio_no_tls.hpp>
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

    private:
    websocketpp::server<websocketpp::config::asio> server;
    std::thread listener_thread;
};
