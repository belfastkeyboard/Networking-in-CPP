#ifndef NETWORKING_UTILITIES
#define NETWORKING_UTILITIES 1

#define _WIN32_WINNT _WIN32_WINNT_VISTA

#include <winsock2.h>
#include <ws2tcpip.h>
#include <time.h>
#include <string>
#include <iostream>

#include "status.h"

#define PORT 54000
#define LOCALHOST "127.0.0.1"

#define ZERO 0
#define ONE 1
#define NULLPTR nullptr
#define BUFFER_LEN 4096
#define SERVER_SHUTDOWN_ERROR 10054
#define IS_TRUE true

#define WSA_SUCCESS ZERO
#define RECV_FLAG ZERO
#define SEND_FLAG ZERO

#define DEPRECATED ZERO

#define THREAD_MEM_SIZE ZERO
#define THREAD_START_NO_DELAY ZERO
#define THREAD_NO_SECURITY_ATTRS NULLPTR
#define THREAD_NO_ID NULLPTR
#define THREAD_WAIT_FOR_ALL IS_TRUE

#define WRITE_FDS NULLPTR
#define EXCEPT_FDS NULLPTR
#define TIMEOUT NULLPTR

#define NO_ADDR NULLPTR
#define NO_ADDRLEN NULLPTR

#define FORMAT_SOURCE NULLPTR
#define FORMAT_N_SIZE ZERO
#define FORMAT_ARGUMENTS NULLPTR

#define BUFFER_COUNT ONE
#define SEND_COMPLETION_ROUTINE NULLPTR

typedef int STATUS;

void PrintWSAErrorMessage(int error);

#endif 
