#pragma once

#include <google/protobuf/message.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>
#include <string>
#include <thread>

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class WebsocketClient : public websocketpp::client<websocketpp::config::asio_tls_client>
{
    public:
    WebsocketClient();
    virtual ~WebsocketClient();
    bool connect(const std::string& host_name);
    bool is_connected();
    void run_loop();

    /****
     * Send message to server
     * @param in the message to send
     * @returns true on sucess
     */
    bool send_client_message(const std::string& in);

    /****
     * Retrieve message from server
     * @param out where to place the message that was retrieved
     * @return opcode retrieved from server
     */
    int32_t receive_server_message(message_ptr out);

    virtual void on_message_receive(message_ptr in) = 0;

    protected:
    websocketpp::client<websocketpp::config::asio_tls_client>::connection_ptr con;
    bool connected = false;
    std::thread listener_thread;
};

