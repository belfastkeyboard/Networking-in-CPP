#include "client.hpp"

int main()
{
    ClientTCP Client(PORT, LOCALHOST);

    if (Client.Init())
        return EXIT_FAILURE;

    if (Client.Run())
        return EXIT_FAILURE;

    if (Client.Cleanup())
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}