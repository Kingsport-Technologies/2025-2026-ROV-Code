#include "websocketserver.hpp"

using namespace std;
WebSocketServer::WebSocketServer()
{
    ws.init_asio();
    ws.set_message_handler(std::bind(&WebSocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
    ws.clear_access_channels(websocketpp::log::alevel::all);
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
        math.left_x = data["gamepad"]["axes"][0];
        math.left_y = data["gamepad"]["axes"][1];
        math.run_loop();
        json j = ready_send();
        callback(j);
    }
    if(data["type"] == "ping")
    {
        cout << "Ping" << endl;
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
json WebSocketServer::ready_send()
{
    json j;
    j["thrusters"]["hfrontleft"] = math.horiz_front_left;
    j["thrusters"]["hfrontright"] = math.horiz_front_right;
    j["thrusters"]["hbackleft"] = math.horiz_back_left;
    j["thrusters"]["hbackright"] = math.horiz_back_right;
    return j;
}
void WebSocketServer::send_callback(std::function<void(json)> newCallback)
{
    callback = newCallback;
}