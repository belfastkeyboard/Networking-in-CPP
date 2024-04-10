#ifndef NETWORKING_SERVER_TCP
#define NETWORKING_SERVER_TCP 1

#include "utils/utils.hpp"
#include <string>
#include <cstdio>

namespace TCP
{

    class Server;

    void ConnectionReceived(Server* listener, SOCKET sock, fd_set* set);
    void MessageReceived(Server* listener, SOCKET sockfd, fd_set* set);

    // function pointer typedef
    typedef void(*ConnectionHandler)(Server* listener, SOCKET sock, fd_set* set);
    typedef void(*MessageHandler)(Server* listener, SOCKET sockfd, fd_set* set);

    class Server
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

        Server(int port, MessageHandler mHandler, ConnectionHandler cHandler);
        ~Server();

    };

}


#endif 
