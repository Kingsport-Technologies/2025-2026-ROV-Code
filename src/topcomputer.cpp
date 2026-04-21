#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include <thread>
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
#include "pwmmath.hpp"
#include <string>
#include <QSettings>

using namespace std;
using json = nlohmann::json;

int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);
    qputenv("QT_MEDIA_BACKEND", "gstreamer");
    QApplication app (argc, argv);
    QApplication::setApplicationName("Dreamer Control System");
    QApplication::setApplicationVersion("0.1.0");
    QApplication::setOrganizationName("KTech");
    // Prevent the settings dialog (exec'd before the main window is shown)
    // from triggering a quit when it closes.
    app.setQuitOnLastWindowClosed(false);

    ControllerInterface* controller = new ControllerInterface();
    atomic<bool> running = true;
    thread pollThread([&]() {
        while (running) {
            controller->runLoop();
        }
    });

    PilotWindow* pilot = new PilotWindow(controller);
    pilot->show();
    // Re-wire quit now that the main window is visible.
    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QApplication::quit);
    outgoingserver server_tx;
    QObject::connect(pilot, &PilotWindow::sigReconnect, &server_tx, &outgoingserver::startServer);
    PwmMath* math = new PwmMath();
    QObject::connect(controller, &ControllerInterface::controllerAxisChanged, math, &PwmMath::incomingControllerData);
    QObject::connect(math, &PwmMath::outgoingPWMData, &server_tx, &outgoingserver::sendPwmInstructions);
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&]() {
        running = false;
        pollThread.join();
    });
    
    return app.exec();
}