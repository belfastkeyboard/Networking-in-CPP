#ifndef NETWORKING_CLIENT_TCP
#define NETWORKING_CLIENT_TCP 1

#include "utils/utils.hpp"

namespace TCP
{

    DWORD WINAPI ThreadSendMessage(LPVOID lpParam);

    class Client
    {

    private:

        SOCKET sock;
        int port;
        std::string addr;

    public:

        STATUS Init();
        STATUS Run();
        STATUS Cleanup();

        Client(int port, std::string ip_addr);
        ~Client();

    };

}


#endif 
