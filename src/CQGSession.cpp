#include "CQGSession.h"
#include "WebAPI/webapi_2.pb.h"
#include "WebAPI/account_authorization_2.pb.h"

#include <exception>

CQGSession::CQGSession() {}

bool CQGSession::connect(const std::string& host_name)
{
    if (host_name.empty())
        throw std::invalid_argument("host_name");
    return client.connect(host_name);
};

bool CQGSession::logon(const std::string& user_name, const std::string& password) 
{ 
    account_authorization_2::AccountLogon clientMessage;
    clientMessage.set_username(user_name);
    clientMessage.set_password(password);
    client.send_client_message(&clientMessage);
    return true; 
}
