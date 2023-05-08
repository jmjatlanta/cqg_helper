#pragma once

#include "WebsocketClient.h"

#include <string>
#include <cstdint>

class CQGSession : public WebsocketClient
{
    public:
    CQGSession();
    bool connect(const std::string& host_name);
    bool disconnect();
    bool logon(int32_t account_id, const std::string& user_name, const std::string& password);
    bool is_logged_in();
    bool logoff();

    protected:
    virtual void on_message_receive(message_ptr in);
    virtual void on_logon_response(const WebAPI_2::ServerMsg& in);
    bool is_logged_in_ = false;
};
