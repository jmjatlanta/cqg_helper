#include "mock_server.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;

    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    if (msg->get_payload() == "stop-listening") {
        s->stop_listening();
        return;
    }

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        std::cout << "Echo failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}

void mock_server::run_loop()
{
    try { 
        // Start the ASIO io_service run loop
        server.run();
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

        server.listen(port);

        // Start the server accept loop
        server.start_accept();
        listener_thread = std::thread(&mock_server::run_loop, this);
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception in mock_server::ctor" << std::endl;
    }
}

mock_server::~mock_server()
{
    // stop listener thread
    server.stop_listening();
    listener_thread.join();
};
