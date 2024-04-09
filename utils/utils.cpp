#include "utils.hpp"

void PrintWSAErrorMessage(int error)
{
    DWORD result;
    LPSTR error_message;

    result = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                FORMAT_SOURCE,
                error,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&error_message,
                FORMAT_N_SIZE,
                nullptr);

    if (result != 0)
    {
        warn("Error code: %d.", error);
        warn("Error message: %s", error_message);
    }
}
