#include "client.hpp"

DWORD __stdcall TCP::ThreadSendMessage(LPVOID lpParam)
{
    SOCKET sockfd;
    std::string input;
    int bytes_sent;

    sockfd = *(SOCKET*)lpParam;

    // Do-while loop to send-receive data
    do {
        input.clear();
        
        printf("> ");
        std::getline(std::cin, input);

        if (input.size())
        {
            bytes_sent = send(sockfd, input.c_str(), input.length() + 1, SEND_FLAG);

            if (bytes_sent == SOCKET_ERROR)
            {
                warn("Error sending.", 0);
                PrintWSAErrorMessage(WSAGetLastError());
                
                return EXIT_FAILURE;
            }
        }

    } while (input.length() - 1 > 0);

    return EXIT_SUCCESS;
}

STATUS TCP::Client::Init()
{
    WORD ver;
    WSADATA data;
    SOCKET sockfd;
    int result;
    sockaddr_in addr;

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

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (sockfd == INVALID_SOCKET)
    {
        warn("Socket creation failed!", 0);
        PrintWSAErrorMessage(WSAGetLastError());

        return EXIT_FAILURE;
    }

    good("Socket '%d' created.", sockfd);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(this->port);
    addr.sin_addr.S_un.S_addr = inet_addr(this->addr.c_str());

    result = connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr));

    if (result == SOCKET_ERROR)
    {
        warn("Connection failed.", 0);
        PrintWSAErrorMessage(WSAGetLastError());

        return EXIT_FAILURE;
    }

    good("Connection to '%s' established.", this->addr.c_str());

    this->sock = sockfd;

    return EXIT_SUCCESS;
}

STATUS TCP::Client::Run()
{
    HANDLE threadHANDLE;
    DWORD threadID;
    time_t current_time;
    struct tm *local_time;
    char buffer[BUFFER_LEN];
    int bytes, error;

    threadHANDLE = CreateThread(THREAD_NO_SECURITY_ATTRS, THREAD_MEM_SIZE, ThreadSendMessage, (LPVOID)&this->sock, THREAD_START_NO_DELAY, &threadID);

    if (threadHANDLE == NULL)
    {
        warn("Thread '%lu' creation failed!", threadID);
        return EXIT_FAILURE;
    }

    while (true)
    {
        memset(buffer, 0, BUFFER_LEN);

        bytes = recv(this->sock, buffer, BUFFER_LEN, RECV_FLAG);
        
        time(&current_time);
        local_time = localtime(&current_time);

        if (bytes <= 0)
        {
            error = WSAGetLastError();

            if (error == SERVER_SHUTDOWN_ERROR)
            {
                info("Connection with server lost.", 0);
                break;
            }
            else
            {
                warn("Error in recv(). Exiting.", 0);
                PrintWSAErrorMessage(error);
            }

            return EXIT_FAILURE;
        }
        else
        {
            printf("[%02d:%02d:%02d] Message received: %s", local_time->tm_hour, local_time->tm_min, local_time->tm_sec, buffer);
        }
    }

    // currently threads are not gracefully exited upon server shutdown
    // due to blocking nature of getline in NetworkThreadClientSend()
    // threads are closed with program exit

    return EXIT_SUCCESS;
}

STATUS TCP::Client::Cleanup()
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

TCP::Client::Client(int port, std::string ip_addr) : port(port), addr(ip_addr) {}

TCP::Client::~Client()
{
    WSACleanup();
}
