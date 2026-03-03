#include "outgoingserver.hpp"

outgoingserver::outgoingserver()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddress.sin_family = AF_INET;
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
    serverAddress.sin_addr.s_addr = inet_pton(AF_INET, ip, &(ip));

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    connected = true;
}
void outgoingserver::sendMessage(string content)
{
    const char* message = content.c_str();
    send(clientSocket, message, strlen(message), 0);
}