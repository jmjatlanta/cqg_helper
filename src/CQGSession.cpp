#include "CQGSession.h"
#include "WebAPI/account_authorization_2.pb.h"

#include <exception>

CQGSession::CQGSession() : WebsocketClient() {}

bool CQGSession::connect(const std::string& host_name)
{
    if (host_name.empty())
        throw std::invalid_argument("host_name");
    return WebsocketClient::connect(host_name);
};

/****
 * We received a logon_result
 * @param in the incoming message
 */
void CQGSession::on_logon_result(const WebAPI_2::ServerMsg& in)
{
    const user_session_2::LogonResult logon_result = in.logon_result();
    if ( logon_result.result_code() == user_session_2::LogonResult_ResultCode_RESULT_CODE_SUCCESS)
        is_logged_in_ = true;
}

void CQGSession::on_message_receive(message_ptr in)
{
    std::cout << "CQGSession::on_message_receive called\n";
    // determine the type
    WebAPI_2::ServerMsg serverMsg;
    serverMsg.ParseFromString(in->get_payload());
    if (serverMsg.has_logon_result())
    {
        on_logon_result(serverMsg);
    }
}

bool CQGSession::logon(int32_t account_id, const std::string& user_name, const std::string& password) 
{ 
    WebAPI_2::ClientMsg clientMsg;
    user_session_2::Logon* logon = clientMsg.mutable_logon();
    logon->set_user_name(user_name);
    logon->set_password(password);
    logon->set_client_version("1");
    logon->set_protocol_version_major(WebAPI_2::PROTOCOL_VERSION_MAJOR);
    logon->set_protocol_version_minor(WebAPI_2::PROTOCOL_VERSION_MINOR);
    std::string msg;
    clientMsg.SerializeToString(&msg);
    send_client_message(msg);
    return true; 
}

bool CQGSession::is_logged_in() { return is_logged_in_; }
