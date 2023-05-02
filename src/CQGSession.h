#pragma once

#include "WebsocketClient.h"

#include <string>

class CQGSession
{
    public:
    CQGSession();
    bool connect(const std::string& host_name);
    bool disconnect();
    bool logon(const std::string& user_name, const std::string& password);
    bool logoff();

    protected:
    WebsocketClient client;
};
