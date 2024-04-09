#include "utils/utils.hpp"
#include <string>
#include <cstdio>

class ServerTCP;

void ConnectionReceived(ServerTCP* listener, SOCKET sock, fd_set* set);
void MessageReceived(ServerTCP* listener, SOCKET sockfd, fd_set* set);

// function pointer typedef
typedef void(*ConnectionHandler)(ServerTCP* listener, SOCKET sock, fd_set* set);
typedef void(*MessageHandler)(ServerTCP* listener, SOCKET sockfd, fd_set* set);

class ServerTCP
{

private:

    SOCKET sock;
    int port;
    MessageHandler m_handler;
    ConnectionHandler c_handler;

public:

    STATUS Init();
    STATUS Run();
    STATUS Cleanup();

    // make SendMessage an async operation? actual online transmission will slow down the server otherwise
    void SendMessage(std::string message, SOCKET sockfd);
    void BroadcastMessage(std::string message, fd_set* set, SOCKET sockfd);

    ServerTCP(int port, MessageHandler mHandler, ConnectionHandler cHandler);
    ~ServerTCP();

};