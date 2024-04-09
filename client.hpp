#ifndef NETWORKING_CLIENT_TCP
#define NETWORKING_CLIENT_TCP 1

#include "utils/utils.hpp"

DWORD WINAPI ThreadSendMessage(LPVOID lpParam);

class ClientTCP
{

private:

    SOCKET sock;
    int port;
    std::string addr;

public:

    STATUS Init();
    STATUS Run();
    STATUS Cleanup();

    ClientTCP(int port, std::string ip_addr);
    ~ClientTCP();

};

#endif 
