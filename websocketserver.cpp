#include "websocketserver.hpp"

using namespace std;
WebSocketServer::WebSocketServer()
{
    ws.init_asio();
    ws.set_message_handler(std::bind(&WebSocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
    
}
void WebSocketServer::start_server()
{
    ws.listen(9002);
    ws.start_accept();
    ws.run();
}
void WebSocketServer::on_message(websocketpp::connection_hdl hdl,server::message_ptr msg)
{
    server* s = &(WebSocketServer::ws);
    string current_data = msg->get_payload();
    json data = json::parse(current_data);
    if(data["type"] == "command")
    {
        WebSocketServer::math.left_x = data["gamepad"]["axes"][0];
        WebSocketServer::math.left_y = data["gamepad"]["axes"][1];
        WebSocketServer::math.run_loop();
        json j;
        j["thrusters"]["horiz_front_left"];
        j["thrusters"];
    }
    if(data["type"] == "ping")
    {
        websocketpp::lib::error_code ec;
        s->send(hdl, "Ping", msg->get_opcode(), ec);
        if (ec) {
            std::cout << "Send error: " << ec.message() << std::endl;
        }
    }
    else
    {
        // cout << "Received" << current_data << endl;
    }
}