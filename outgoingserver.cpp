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

    if(connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    connected = true;
}
void outgoingserver::sendMessage(string content)
{
    const char* message = content.c_str();
    send(clientSocket, message, strlen(message), 0);
}