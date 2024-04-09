#include "server.hpp"

int main()
{
    ServerTCP Server(PORT, MessageReceived, ConnectionReceived);

    if (Server.Init())
    {
        return EXIT_FAILURE;
    }   

    if (Server.Run())
    {
        return EXIT_FAILURE;
    }

    if (Server.Cleanup())
    {
        return EXIT_FAILURE;
    }

    return 0;
}

// define server functions here...
void MessageReceived(ServerTCP* listener, SOCKET sockfd, fd_set* set)
{
    char buffer[BUFFER_LEN];
    int bytes_rcvd;
    time_t current_time;
    struct tm* local_time;
    std::string message;

    time(&current_time);
    local_time = localtime(&current_time);

    info("[%02d:%02d:%02d] Incoming message.", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    memset(buffer, 0, BUFFER_LEN);

    bytes_rcvd = recv(sockfd, buffer, BUFFER_LEN, RECV_FLAG);

    time(&current_time);
    local_time = localtime(&current_time);

    if (bytes_rcvd <= 0)
    {
        info("[%02d:%02d:%02d] Client '%d' disconnected.", local_time->tm_hour, local_time->tm_min, local_time->tm_sec, sockfd);
        closesocket(sockfd); // error handling later
        FD_CLR(sockfd, set);

        return;
    }

    message = buffer;
    listener->BroadcastMessage(message, set, sockfd);

    return;
}

void ConnectionReceived(ServerTCP* listener, SOCKET sock, fd_set* set)
{
    SOCKET sockfd;
    time_t current_time;
    tm* local_time;
    std::string message;

    time(&current_time);
    local_time = localtime(&current_time);

    info("[%02d:%02d:%02d] Incoming connection.", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
                
    sockfd = accept(sock, NO_ADDR, NO_ADDRLEN);

    time(&current_time);
    local_time = localtime(&current_time);

    if (sockfd == INVALID_SOCKET)
    {
        warn("[%02d:%02d:%02d] Error accepting connection.", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        PrintWSAErrorMessage(WSAGetLastError());

        return;
    }

    FD_SET(sockfd, set);
    info("[%02d:%02d:%02d] New user accepted.", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

    message = "Welcome to the chat!\n";
    listener->SendMessage(message, sockfd);

    message = "A new user has joined!\n";
    listener->BroadcastMessage(message, set, sockfd);
}
