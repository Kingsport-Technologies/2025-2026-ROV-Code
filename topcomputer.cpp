#include <string>
#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;
void on_message(server* s,
                websocketpp::connection_hdl hdl,
                server::message_ptr msg)
{
    string current_data = msg->get_payload();
    json data = json::parse(current_data);
    if(data["type"] == "command")
    {
        // cout << "Received Command" << endl;
    }
    if(data["type"] == "ping")
    {
        cout << "Ping" << endl;
    }
    else
    {
        cout << "Received" << current_data << endl;
    }
}

int main(int argc, char* argv[])
{
    server ws;
    ws.init_asio();
    ws.set_message_handler(std::bind(&on_message, &ws, std::placeholders::_1, std::placeholders::_2));
    ws.listen(9002);
    ws.start_accept();
    ws.run();
}