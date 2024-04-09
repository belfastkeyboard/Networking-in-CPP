#ifndef NETWORKING_UTILITIES
#define NETWORKING_UTILITIES 1

#define _WIN32_WINNT _WIN32_WINNT_VISTA

#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <stdbool.h>

#include "status.h"

#define PORT 54000
#define LOCALHOST "127.0.0.1"

#define ZERO 0
#define BUFFER_LEN 4096
#define SERVER_SHUTDOWN_ERROR 10054

#define WSA_SUCCESS ZERO
#define RECV_FLAG ZERO
#define SEND_FLAG RECV_FLAG

#define DEPRECATED ZERO

#define THREAD_MEM_SIZE ZERO
#define THREAD_START_NO_DELAY ZERO
#define THREAD_NO_SECURITY_ATTRS NULL
#define THREAD_NO_ID NULL
#define THREAD_WAIT_FOR_ALL TRUE

typedef int STATUS;

typedef struct ThreadArgs
{
    SOCKET sockfd;
    bool* exit;
} ThreadArgs;

void PrintWSAErrorMessage(int error);

STATUS NetworkWSAInitialisation(WSADATA* wsa_data);
STATUS NetworkWSACleanup(void);

STATUS NetworkCreateSocket(SOCKET* sockfd, int af, int type, int protocol);
STATUS NetworkCloseSocket(SOCKET sockfd);
STATUS NetworkBindSocket(SOCKET sockfd, struct sockaddr_in* addr);
STATUS NetworkListenSocket(SOCKET sockfd, int backlog);
STATUS NetworkConnect(SOCKET sockfd, struct sockaddr_in* addr);
STATUS NetworkWaitForConnection(struct sockaddr_in* addr, SOCKET serv, SOCKET* recv);

STATUS NetworkServerReceive(SOCKET sockfd);
STATUS NetworkClientSend(SOCKET sockfd);

STATUS NetworkMultiServerReceive(SOCKET sock_serv);

// threaded functions
DWORD WINAPI NetworkThreadClientSend(LPVOID lpParam);
DWORD WINAPI NetworkThreadClientReceive(LPVOID lpParam);
STATUS NetworkClientSendReceive(SOCKET sockfd);

void NetworkConstructSockaddr_in(struct sockaddr_in* addr, short fam, u_short port, u_long S_addr);

#endif 
