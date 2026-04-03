#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include "websocketserver.hpp"
#include "outgoingserver.hpp"
#include "json.hpp"
#include <functional>
#include <QQmlApplicationEngine>
#include "pilot_window.hpp"
#include <QApplication>
#include <gst/gst.h>

using namespace std;
using json = nlohmann::json;
typedef websocketpp::server<websocketpp::config::asio> server;

WebSocketServer server_rx;
outgoingserver server_tx;
int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);
    QApplication app (argc, argv);
    QApplication::setApplicationName("Dreamer Control System");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setOrganizationName("KTech");
    PilotWindow* pilot = new PilotWindow();
    pilot->show();
    return app.exec();

    // server_rx.send_callback(
    //     [](nlohmann::json content) {
    //         server_tx.sendMessage(content.dump());
    //     }
    // );

    // // Run websocket server in a separate thread so it doesn't block
    // std::thread ws_thread([](){ server_rx.start_server(); });
    
    // server_tx.start(argv[1], std::stoi(argv[2]));
    
    // ws_thread.join();
}