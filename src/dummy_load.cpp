#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>


bool connected = false;
int serverSocket;
void signal_callback_handler(int signum)
{
    if(signum == 2 and connected)
    {
        close(serverSocket);
        std::cout << "Socket Successfully Closed" << std::endl;
    }
    exit(signum);
}
int main(int argc, char* argv[])
{
    signal(SIGINT, signal_callback_handler);
    if(argc != 2)
    {
        std::cout << "Remeber, the port is a required argument" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);
    std::cout << "Listining on " << port << std::endl;
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    connected = true;
    std::cout << "Client Connected";
    while (true) 
    {
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << buffer << std::endl;
    }

    return 0;
}