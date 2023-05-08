#include "mock_server.h"
#include "WebAPI/webapi_2.pb.h"

#include <iostream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef wsserver::message_ptr message_ptr;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

void on_login(wsserver* s, websocketpp::connection_hdl hdl, const WebAPI_2::ClientMsg& msg)
{
    std::cout << "mock_server::on_login returning ServerMsg\n";
    websocketpp::lib::error_code ec;
    WebAPI_2::ServerMsg out;
    // TODO: Make a logon response and send
    user_session_2::LogonResult* lr = out.mutable_logon_result();
    lr->set_result_code(user_session_2::LogonResult_ResultCode_RESULT_CODE_SUCCESS);
    lr->set_base_time("2023-01-01T12:00:00");
    lr->set_server_time(1);
    lr->set_protocol_version_minor(1);
    lr->set_protocol_version_major(1);
    std::string str;
    out.SerializeToString(&str);
    s->send(hdl, str, websocketpp::frame::opcode::binary, ec);
}

// Define a callback to handle incoming messages
void on_message(wsserver* s, websocketpp::connection_hdl hdl, wsserver::message_ptr msg) {
    std::cout << "mock_server::on_message called with message of " 
                << msg->get_payload().size() << " bytes\n";

    try {
        WebAPI_2::ClientMsg clientMessage;
        if (clientMessage.ParseFromString(msg->get_payload())
                && clientMessage.has_logon())
            on_login(s, hdl, clientMessage);
        else
            std::cout << "mock_server::on_message: is not a logon message\n";
    } catch (websocketpp::exception const & e) {
        std::cout << "mock_server::on_message failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}
// called when http connection arrives
void on_http(wsserver* s, websocketpp::connection_hdl hdl) {
    wsserver::connection_ptr con = s->get_con_from_hdl(hdl);
    
    con->set_body("Hello World!");
    con->set_status(websocketpp::http::status_code::ok);
    std::cout << "mock_server::on_http called\n";
}

std::string get_password() {
    std::cout << "mock_server::get_password called\n";
    return "test";
}
// See https://wiki.mozilla.org/Security/Server_Side_TLS for more details about
// the TLS modes. The code below demonstrates how to implement both the modern
enum tls_mode {
    MOZILLA_INTERMEDIATE = 1,
    MOZILLA_MODERN = 2
};
// called when negotiating SSL
context_ptr on_tls_init(tls_mode mode, websocketpp::connection_hdl hdl) {
    std::cout << "mock_server::on_tls_init called\n";
    namespace asio = websocketpp::lib::asio;

    std::cout << "on_tls_init called with hdl: " << hdl.lock().get() << std::endl;
    std::cout << "using TLS mode: " << (mode == MOZILLA_MODERN ? "Mozilla Modern" : "Mozilla Intermediate") << std::endl;

    context_ptr ctx = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);

    try {
        if (mode == MOZILLA_MODERN) {
            // Modern disables TLSv1
            ctx->set_options(asio::ssl::context::default_workarounds |
                             asio::ssl::context::no_sslv2 |
                             asio::ssl::context::no_sslv3 |
                             asio::ssl::context::no_tlsv1 |
                             asio::ssl::context::single_dh_use);
        } else {
            ctx->set_options(asio::ssl::context::default_workarounds |
                             asio::ssl::context::no_sslv2 |
                             asio::ssl::context::no_sslv3 |
                             asio::ssl::context::single_dh_use);
        }
        ctx->set_password_callback(bind(&get_password));
        ctx->use_certificate_chain_file("../test/server.pem");
        ctx->use_private_key_file("../test/server.pem", asio::ssl::context::pem);
        
        // Example method of generating this file:
        // `openssl dhparam -out dh.pem 2048`
        // Mozilla Intermediate suggests 1024 as the minimum size to use
        // Mozilla Modern suggests 2048 as the minimum size to use.
        ctx->use_tmp_dh_file("../test/dh.pem");
        
        std::string ciphers;
        
        if (mode == MOZILLA_MODERN) {
            ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";
        } else {
            ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:DES-CBC3-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA";
        }
        
        if (SSL_CTX_set_cipher_list(ctx->native_handle() , ciphers.c_str()) != 1) {
            std::cout << "Error setting cipher list" << std::endl;
        }
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    std::cout << "mock_server::on_tls_init completed\n";
    return ctx;
}

void mock_server::run_loop()
{
    std::cout << "In mock_server::run_loop\n";
    try { 
        running = true;
        // Start the ASIO io_service run loop
        server.run();
        std::cout << "mock_server::run_loop unblocked\n";
    } catch (websocketpp::exception const& e) {
        std::cout << "run_loop failed because: "
            << "(" << e.what() << ")" << std::endl;
    } catch (...) {
        std::cout << "run_loop failed for unknown reason\n";
    }
}

mock_server::mock_server(int32_t port)
{
    // start the message thread
    try {
        // Set logging settings
        server.set_access_channels(websocketpp::log::alevel::all);
        server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        server.init_asio();

        // Register our message handler
        server.set_message_handler(bind(&on_message,&server,::_1,::_2));
        server.set_http_handler(bind(&on_http,&server,::_1));
        server.set_tls_init_handler(bind(&on_tls_init,MOZILLA_INTERMEDIATE,::_1));

        server.listen(port);

        // Start the server accept loop
        server.start_accept();

        // start listener loop
        listener_thread = std::thread(&mock_server::run_loop, this);
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception in mock_server::ctor" << std::endl;
    }
}

mock_server::~mock_server()
{
    std::cout << "mock_server dtor called\n";
    // stop listener thread
    server.stop_listening();
    listener_thread.join();
};
