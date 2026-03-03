#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

using namespace std;
class outgoingserver
{
    public:
        outgoingserver();
        ~outgoingserver();
        void sendMessage(string message);
        void start(const char* ip, int port);
    private:
        int clientSocket;
        sockaddr_in serverAddress;
        bool connected;

};