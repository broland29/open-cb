#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include "Validator.h"
#include "Logger.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55555

class Client
{
public:
    Client();
    int runLoop();

private:
    void validateNewBoard(char recvBuffer[200]);
    void getCurrentBoard();
    void startNewGame();

public:

private:
    Validator* validator;
    SOCKET clientSocket;
};