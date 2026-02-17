#include <string>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <chrono>
#include "websocketserver.hpp"

using namespace std;
WebSocketServer server;

int main(int argc, char* argv[])
{
    server.start_server();
}