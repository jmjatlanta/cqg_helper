#pragma once

#include <string>
#include <google/protobuf/message.h>

class WebsocketClient
{
    public:
    WebsocketClient() {}
    bool connect(const std::string& host_name) { return connected; }
    bool is_connected() { return connected;}
    bool send_client_message(const google::protobuf::Message* msg) { return true; }
    protected:
    bool connected = false;
};

