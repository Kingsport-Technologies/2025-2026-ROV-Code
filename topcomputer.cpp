#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include "websocketserver.hpp"
#include "outgoingserver.hpp"
#include "json.hpp"
#include <functional>

using namespace std;
using json = nlohmann::json;
typedef websocketpp::server<websocketpp::config::asio> server;

WebSocketServer server_rx;
outgoingserver server_tx;
int main(int argc, char* argv[])
{
    server_rx.send_callback(
        [](nlohmann::json content) {
            server_tx.sendMessage(content.dump());
        }
    );

    server_rx.start_server();
    server_tx.start(argv[1], std::stoi(argv[2]));
}