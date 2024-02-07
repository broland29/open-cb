#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>
#include <QtCore>
#include <QDebug>

#define IP                  "127.0.0.1"
#define PORT                55555
#define SEND_BUFFER_SIZE    200
#define RECV_BUFFER_SIZE    200

class Server : public QThread
{
private:
    SOCKET serverSocket;
    SOCKET acceptSocket;
    char name[10];

public:
    Server(const char name[10]);

    // initializes socket and connects to one client
    int connect();

    int receiveMessage(char board[8][8], char& cmd);

    int sendMessage(char message[SEND_BUFFER_SIZE]);

    int close();

    // override QThread run
    void run();
};
