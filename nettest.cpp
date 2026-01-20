#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(server* s,
                websocketpp::connection_hdl hdl,
                server::message_ptr msg)
{
    std::cout << "Received: " << msg->get_payload() << std::endl;
}

int main() {
    server ws;
    ws.init_asio();
    ws.set_message_handler(std::bind(&on_message, &ws, std::placeholders::_1, std::placeholders::_2));
    ws.listen(9002);
    ws.start_accept();
    ws.run();
}
