#include "Client.h"
#include "Logger.h"

int main()
{
    Logger::initialize();

    try
    {
        Client client;
        return client.runLoop();
    }
    catch (int exceptionNumber)
    {
        SPDLOG_ERROR("Failed to construct and run client!");
        return exceptionNumber;
    }
}
