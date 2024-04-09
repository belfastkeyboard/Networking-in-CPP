#include "server.hpp"

STATUS ServerTCP::Init()
{
    WORD ver;
    WSADATA data;
    SOCKET sockfd;
    sockaddr_in addr;
    int result;

    if (this->port < 49152 || this->port > 65535)
    {
        warn("Port outside acceptable range.", 0);

        return EXIT_FAILURE;
    }

    good("Port '%d' set.", this->port);
    
    ver = MAKEWORD(2, 2);
    result = WSAStartup(ver, &data);

    if (result != WSA_SUCCESS)
    {
        warn("WSA init error.", 0);
        PrintWSAErrorMessage(result);

        return EXIT_FAILURE;
    }

    good("WSA initialised.", 0);

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd == INVALID_SOCKET)
    {
        warn("Socket creation error.", 0);
        PrintWSAErrorMessage(WSAGetLastError());
        
        return EXIT_FAILURE;
    }

    good("Socket '%d' created.", sockfd);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->port);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // specific IP addr?

    result = bind(sockfd, (const struct sockaddr*)&addr, sizeof(addr));

    if (result != WSA_SUCCESS)
    {
        warn("Socket bind error.", 0);
        PrintWSAErrorMessage(WSAGetLastError());

        closesocket(sockfd);
        WSACleanup();

        return EXIT_FAILURE;
    }

    good("Socket '%d' bound.", sockfd);

    result = listen(sockfd, SOMAXCONN);

    if (result != WSA_SUCCESS)
    {
        warn("Socket listen error.", 0);
        PrintWSAErrorMessage(WSAGetLastError());

        closesocket(sockfd);
        WSACleanup();

        return EXIT_FAILURE;
    }

    good("Socket '%d' set to state: listening.", sockfd);

    this->sock = sockfd;

    good("Server initialised.");

    return EXIT_SUCCESS;
}

STATUS ServerTCP::Run()
{
    int sockets;
    fd_set master;
    SOCKET sockfd;

    FD_ZERO(&master);
    FD_SET(this->sock, &master);

    while (true)
    {
        fd_set worker;

        worker = master;

        printf("Connections: %d.\n", master.fd_count);

        sockets = select(DEPRECATED, &worker, WRITE_FDS, EXCEPT_FDS, TIMEOUT); // timeout ?
        // select error handling here

        for (int i = 0; i < sockets; i++)
        {
            sockfd = worker.fd_array[i];

            if (sockfd == this->sock)
                this->c_handler(this, this->sock, &master);
            else
                this->m_handler(this, sockfd, &master);
        }

        // exit condition to shut down the server
        if (false)
            break;
    }

    for (int i = 0; i < master.fd_count;) // does this condition actually clear?
    {
        sockfd = master.fd_array[i];

        FD_CLR(sockfd, &master);
        closesocket(sockfd);
    }

    return EXIT_SUCCESS;
}

STATUS ServerTCP::Cleanup()
{
    int result;

    result = closesocket(this->sock);

    if (result == SOCKET_ERROR)
    {
        warn("Socket close error.", 0);
        PrintWSAErrorMessage(WSAGetLastError());

        WSACleanup();

        return result;
    }

    result = WSACleanup();

    if (result != WSA_SUCCESS)
    {
        warn("Failed WSA cleanup.", 0);
        PrintWSAErrorMessage(WSAGetLastError());
    }

    return result;
}

void ServerTCP::SendMessage(std::string message, SOCKET sockfd)
{
    int result;
    time_t current_time;
    tm* local_time;
    WSABUF buff;
    DWORD bytes_sent;
    OVERLAPPED overlapped;


    buff.buf = (char*)message.c_str();
    buff.len = message.length();

    memset(&overlapped, 0, sizeof(overlapped));

    // bytes = send(sockfd, message.c_str(), message.size(), SEND_FLAG);
    result = WSASend(sockfd, &buff, BUFFER_COUNT, &bytes_sent, SEND_FLAG, &overlapped, SEND_COMPLETION_ROUTINE);

    time(&current_time);
    local_time = localtime(&current_time);

    // if (bytes <= 0)
    if (result != WSA_SUCCESS)
    {
        warn("[%02d:%02d:%02d] Message failed to send", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
        return;
    }

    message = message.substr(0, (message.length() < 40) ? message.size() - 1 : 40);

    printf("[%02d:%02d:%02d] Message '%s' sent to socket '%d'.\n", local_time->tm_hour, local_time->tm_min, local_time->tm_sec, message.c_str(), sockfd);
}

void ServerTCP::BroadcastMessage(std::string message, fd_set* set, SOCKET sockfd)
{
    SOCKET user;

    for (int i = 0; i < set->fd_count; i++)
    {
        user = set->fd_array[i];

        if (user == sockfd || user == this->sock)
            continue;

        this->SendMessage(message, user);
    }
}

ServerTCP::ServerTCP(int port, MessageHandler mHandler, ConnectionHandler cHandler) : port(port), m_handler(mHandler), c_handler(cHandler) {}

ServerTCP::~ServerTCP()
{
    WSACleanup();
}
