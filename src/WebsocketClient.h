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
    bool send_client_message(const google::protobuf::Message* in);
    /****
     * Retrieve message from server
     * @param out where to place the message that was retrieved
     * @return opcode retrieved from server
     */
    int32_t receive_server_message(google::protobobuf::Message** out);

    protected:
    websocketpp::client<websocketpp::config::asio_tls_client> client;
    websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr con;
    bool connected = false;
    std::thread listener_thread;
};

