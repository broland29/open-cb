#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>

#define IP                  "127.0.0.1"
#define PORT                55555
#define SEND_BUFFER_SIZE    200
#define RECV_BUFFER_SIZE    200

struct Server
{
    SOCKET serverSocket;
    SOCKET acceptSocket;

    Server();

    // initializes socket and connects to one client
    int connect();

    int receiveMessage(char board[8][8], char& cmd);

    int sendMessage(char message[SEND_BUFFER_SIZE]);

    int close();
};
