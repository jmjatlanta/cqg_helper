#include "WebsocketClient.h"

#include <iostream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

/******
 * This message handler will be invoked once for each incoming message. It
 *  prints the message and then sends a copy of the message back to the server.
 */
void on_message(wsclient* c, websocketpp::connection_hdl hdl, wsclient::message_ptr msg) {
    std::cout << "Client on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    WebsocketClient& client = static_cast<WebsocketClient&>(*c);
    client.on_message_receive(msg);
}

/// Verify that one of the subject alternative names matches the given hostname
bool verify_subject_alternative_name(const char * hostname, X509 * cert) {
    STACK_OF(GENERAL_NAME) * san_names = NULL;
    
    san_names = (STACK_OF(GENERAL_NAME) *) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
    if (san_names == NULL) {
        std::cout << "client AN stack null\n";
        return false;
    }
    
    int san_names_count = sk_GENERAL_NAME_num(san_names);
    
    bool result = false;
    
    for (int i = 0; i < san_names_count; i++) {
        const GENERAL_NAME * current_name = sk_GENERAL_NAME_value(san_names, i);
        
        if (current_name->type != GEN_DNS) {
            continue;
        }
        
        char const * dns_name = (char const *) ASN1_STRING_get0_data(current_name->d.dNSName);
        
        // Make sure there isn't an embedded NUL character in the DNS name
        if (ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name)) {
            break;
        }
        // Compare expected hostname with the CN
        result = (strcasecmp(hostname, dns_name) == 0);
    }
    sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);

    if (!result)
        std::cout << "client verify_subject_an returning false\n";
    
    return result;
}

/// Verify that the certificate common name matches the given hostname
bool verify_common_name(char const * hostname, X509 * cert) {
    // Find the position of the CN field in the Subject field of the certificate
    int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name(cert), NID_commonName, -1);
    if (common_name_loc < 0) {
        std::cout << "client common_name_loc is " << common_name_loc << "\n";
        return false;
    }
    
    // Extract the CN field
    X509_NAME_ENTRY * common_name_entry = X509_NAME_get_entry(X509_get_subject_name(cert), common_name_loc);
    if (common_name_entry == NULL) {
        std::cout << "client common_name_entry is null\n";
        return false;
    }
    
    // Convert the CN field to a C string
    ASN1_STRING * common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
    if (common_name_asn1 == NULL) {
        std::cout << "client common_name_asn1 is null\n";
        return false;
    }
    
    char const * common_name_str = (char const *) ASN1_STRING_get0_data(common_name_asn1);
    
    // Make sure there isn't an embedded NUL character in the CN
    if (ASN1_STRING_length(common_name_asn1) != strlen(common_name_str)) {
        std::cout << "client common_name_asn1 has null character\n";
        return false;
    }
    
    if (strcasecmp(hostname, common_name_str) != 0)
        std::cout << "client hostname mismatch\n";

    // Compare expected hostname with the CN
    return (strcasecmp(hostname, common_name_str) == 0);
}

/**
 * This code is derived from examples and documentation found ato00po
 * http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/example/cpp03/ssl/client.cpp
 * and
 * https://github.com/iSECPartners/ssl-conservatory
 */
bool verify_certificate(const char * hostname, bool preverified, boost::asio::ssl::verify_context& ctx) {

    // TODO: Bypassing for now, but need to verify cert
    return true;

    // The verify callback can be used to check whether the certificate that is
    // being presented is valid for the peer. For example, RFC 2818 describes
    // the steps involved in doing this for HTTPS. Consult the OpenSSL
    // documentation for more details. Note that the callback is called once
    // for each certificate in the certificate chain, starting from the root
    // certificate authority.

    // Retrieve the depth of the current cert in the chain. 0 indicates the
    // actual server cert, upon which we will perform extra validation
    // (specifically, ensuring that the hostname matches. For other certs we
    // will use the 'preverified' flag from Asio, which incorporates a number of
    // non-implementation specific OpenSSL checking, such as the formatting of
    // certs and the trusted status based on the CA certs we imported earlier.
    int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());

    // if we are on the final cert and everything else checks out, ensure that
    // the hostname is present on the list of SANs or the common name (CN).
    if (depth == 0 && preverified) {
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());

        if (verify_subject_alternative_name(hostname, cert)) {
            return true;
        } else if (verify_common_name(hostname, cert)) {
            return true;
        } else {
            std::cout << "client unable to verify common name\n";
            return false;
        }
    }

    if (!preverified)
        std::cout << "client not preverified\n";

    return preverified;
}
context_ptr on_tls_init(const char * hostname, websocketpp::connection_hdl) {
    context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);


        ctx->set_verify_mode(boost::asio::ssl::verify_peer);
        ctx->set_verify_callback(bind(&verify_certificate, hostname, ::_1, ::_2));

        // Here we load the CA certificates of all CA's that this client trusts.
        ctx->load_verify_file("../test/ca-chain.cert.pem");
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return ctx;
}

WebsocketClient::WebsocketClient() : wsclient() {}

WebsocketClient::~WebsocketClient() 
{
    // kill loop
    try {
        websocketpp::lib::error_code ec;
        std::string reason = "Client closing connection";
        websocketpp::close::status::value code = websocketpp::close::status::going_away;
        close(con->get_handle(), code, reason, ec);
        listener_thread.join();
    } catch (websocketpp::exception const & e) {
        std::cout << "client dtor: " << e.what() << std::endl;
        listener_thread.detach();
    }
}

void WebsocketClient::run_loop()
{
    try
    {
        run();
    } catch (websocketpp::exception const & e) {
        std::cout << "client websocket exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "client unknown exception\n";
    }
}

bool WebsocketClient::connect(const std::string& host_name) 
{ 
    try {
        // Set logging to be pretty verbose (everything except message payloads)
        set_access_channels(websocketpp::log::alevel::all);
        clear_access_channels(websocketpp::log::alevel::frame_payload);
        set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        init_asio();

        // Register our message handler
        set_message_handler(bind(&on_message,this,::_1,::_2));
        set_tls_init_handler(bind(&on_tls_init,host_name.c_str(), ::_1));

        websocketpp::lib::error_code ec;
        con = get_connection(host_name, ec);
        if (ec) {
            std::cout << "client: could not create connection because: " << ec.message() << std::endl;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        wsclient::connect(con);
        connected = true;

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        listener_thread = std::thread(&WebsocketClient::run_loop, this);
    } catch (websocketpp::exception const & e) {
        std::cout << "client::connect():  " << e.what() << std::endl;
    }
    return connected; 
}

bool WebsocketClient::is_connected() { return connected;}
bool WebsocketClient::send_client_message(const std::string& in) 
{ 
    std::cout << "WebsocketClient::send_client_message: Sending " << in.size() << " bytes.\n";
    websocketpp::lib::error_code ec;
    try {
        send(con->get_handle(), in, websocketpp::frame::opcode::binary, ec);
        if (ec)
        {
            std::cout << "WebsocketClient::send_client_message error code " << ec.message() << std::endl;
        }
    } catch (websocketpp::exception const & e) {
        std::cout << "WebsocketClient::send_client_message failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
    return true; 
}
