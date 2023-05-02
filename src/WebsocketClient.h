#pragma once

#include <google/protobuf/message.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <string>
#include <thread>

class WebsocketClient
{
    public:
    WebsocketClient();
    ~WebsocketClient();
    bool connect(const std::string& host_name);
    bool is_connected();
    void run_loop();
    bool send_client_message(const google::protobuf::Message* msg);

    protected:
    websocketpp::client<websocketpp::config::asio_tls_client> client;
    websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr con;
    bool connected = false;
    std::thread listener_thread;
};

