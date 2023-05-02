#include "CQGSession.h"
#include "WebAPI/webapi_2.pb.h"
#include "WebAPI/account_authorization_2.pb.h"

#include <exception>

CQGSession::CQGSession() {}

bool CQGSession::connect()
{
    if (host_name.empty())
        throw std::invalid_argument("host_name");
    return client.connect(host_name);
};

bool CQGSession::logon() 
{ 
    account_authorization_2::AccountLogon clientMessage;
    clientMessage.set_username(this->user_name);
    clientMessage.set_password(this->password);
    client.send_client_message(&clientMessage);
    return false; 
}
