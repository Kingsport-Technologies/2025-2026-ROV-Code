#include "outgoingserver.hpp"

outgoingserver::outgoingserver()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
    connected = false;
}

outgoingserver::~outgoingserver()
{
    if(connected)
    {
        close(clientSocket);
    }
}

void outgoingserver::start(const char* ip, int port)
{
    serverAddress.sin_port = htons(port);
    // correctly convert the textual IP to binary form
    int ret = inet_pton(AF_INET, ip, &serverAddress.sin_addr);
    if(ret <= 0) {
        if(ret == 0)
            std::cerr << "Invalid address format: " << ip << std::endl;
        else
            perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if(::connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    connected = true;
    std::cout << "connected";
    emit changedConnectionStatus(true);
}
void outgoingserver::sendMessage(string content)
{
    if (!connected) return;
    const char* message = content.c_str();
    send(clientSocket, message, strlen(message), MSG_NOSIGNAL);
    struct tcp_info info;
    socklen_t len = sizeof(info);
    if (getsockopt(clientSocket, SOL_TCP, TCP_INFO, &info, &len) == 0) {
        rtt_ms = info.tcpi_rtt / 1000.0;
        emit changedPing(rtt_ms);
    }
}
void outgoingserver::sendPwmInstructions(int hfl, int hbl, int hfr, int hbr, int vl, int vr)
{
    json j;
    j["type"] = "pwm";
    j["thrusters"]["hfl"] = hfl;
    j["thrusters"]["hbl"] = hbl;
    j["thrusters"]["hfr"] = hfr;
    j["thrusters"]["hbr"] = hbr;
    j["thrusters"]["vl"] = vl;
    j["thrusters"]["vr"] = vr;

    sendMessage(j.dump());
}
void outgoingserver::startServer()
{
    if(!connected)
    {
        QSettings* settings = new QSettings("KTech", "2526Dreamer");
        start(settings->value("robot/ip").toString().toLocal8Bit().data(), settings->value("robot/port").toInt());

    }
}
double outgoingserver::getPing()
{
    return rtt_ms;
}
