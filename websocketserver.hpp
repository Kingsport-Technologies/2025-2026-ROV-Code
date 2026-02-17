#include "json.hpp"
#include "pwmmath.hpp"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <string>
#include <iostream>

using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;

class WebSocketServer
{
    public:   
        WebSocketServer();
        void on_message(websocketpp::connection_hdl hdl,server::message_ptr msg);
    protected:
        server ws;
        PwmMath math;

};