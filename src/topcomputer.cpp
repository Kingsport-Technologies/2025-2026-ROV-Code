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
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include "controller_interface.hpp"
#include <atomic>

using namespace std;
using json = nlohmann::json;
typedef websocketpp::server<websocketpp::config::asio> server;

WebSocketServer server_rx;
outgoingserver server_tx;
int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);
    qputenv("QT_MEDIA_BACKEND", "gstreamer");
    
    ControllerInterface* interface = new ControllerInterface();
    QObject::connect(interface, &ControllerInterface::controllerAxisChanged, [](std::vector<Sint16> values) {
        std::string out = "[";
        for (size_t i = 0; i < values.size(); ++i) {
            if (i > 0) out += ", ";
            out += std::to_string(values[i]);
        }
        out += "]";
        std::cout << "controllerAxisChanged: " << out << std::endl;
    });
    atomic<bool> running = true;

    thread pollThread([&]() {
        while (running) {
            interface->runLoop();
        }
    });
    QApplication app (argc, argv);
    QApplication::setApplicationName("Dreamer Control System");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setOrganizationName("KTech");
    PilotWindow* pilot = new PilotWindow();
    pilot->show();
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        running = false;
        pollThread.join();
    });
    return app.exec();
}