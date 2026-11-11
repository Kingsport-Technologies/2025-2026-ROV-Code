#include "json.hpp"
#include "pwmmath.hpp"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>
#include <iostream>
#include <functional>

using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;

class WebSocketServer
{
    public:   
        WebSocketServer();
        void on_message(websocketpp::connection_hdl hdl,server::message_ptr msg);
        void start_server();
        json ready_send();
        void send_callback(std::function<void(json)> newCallback);
    protected:
        server ws;
        PwmMath math;
        std::function<void(json)> callback;

};