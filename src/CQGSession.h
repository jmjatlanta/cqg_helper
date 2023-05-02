#pragma once

#include "WebsocketClient.h"

#include <string>

class CQGSession
{
    public:
    CQGSession();
    bool connect();
    bool disconnect();
    bool logon();
    bool logoff();

    public:
    std::string host_name;
    std::string user_name;
    std::string password;

    protected:
    WebsocketClient client;
};
