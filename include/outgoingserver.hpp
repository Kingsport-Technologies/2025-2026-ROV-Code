#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include <QObject>
#include "json.hpp"
#include <QSettings>

using json = nlohmann::json;

using namespace std;
class outgoingserver : public QObject
{
    Q_OBJECT
    public:
        outgoingserver();
        ~outgoingserver();
        void sendMessage(string message);
        void start(const char* ip, int port);
    public slots:
        void sendPwmInstructions(int hfl, int hbl, int hfr, int hbr, int vl, int vf);
        // void sendClawInstructions()
        void startServer();
    private:
        int clientSocket;
        sockaddr_in serverAddress;
        bool connected;

};