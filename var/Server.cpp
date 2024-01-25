#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "Server.h"

#define IP      "127.0.0.1"
#define PORT    55555


int server_main()
{
    // initialize WSA
    WORD wVersionRequested = MAKEWORD(2, 2);        // we will use version 2.2
    WSADATA wsaData;                                // structure which will be populated by WSAStartup()
    int wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0)
    {
        std::cout << "WSAStartup() error, code: " << wsaErr << std::endl;
        return 1;
    }
    std::cout << "WSAStartup() success, status: " << wsaData.szSystemStatus << std::endl;

    // create socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "socket() error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "socket() success." << std::endl;

    // bind socket to ip and port
    sockaddr_in service;  // will be casted to older SOCKADDR type
    service.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &service.sin_addr);
    service.sin_port = htons(55555);

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        std::cout << "bind() error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "bind() success." << std::endl;

    // listen for (one) client connection
    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "listen() error: " << WSAGetLastError() << std::endl;
        // return 1
    }
    else
    {
        std::cout << "listen() success, waiting for connections." << std::endl;
    }

    // accept: blocking wait for a connection
    SOCKET acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET)
    {
        std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "accept() success" << std::endl;

    // recieve message
    char recvBuffer[200];
    int recvByteCount = recv(acceptSocket, recvBuffer, 200, 0);
    if (recvByteCount > 0)
    {
        std::cout << "recv() recieved: " << recvBuffer << std::endl;
    }
    else
    {
        std::cout << "recv() error: " << WSAGetLastError();
        WSACleanup();
    }

    // send message
    char sendBuffer[200] = "Message from server";
    int sendByteCount = send(acceptSocket, sendBuffer, 200, 0);
    if (sendByteCount > 0)
    {
        std::cout << "send() success." << std::endl;
    }
    else
    {
        std::cout << "send() error: " << WSAGetLastError();
        WSACleanup();
    }

    // close socket
    std::cout << "Calling closesocket" << std::endl;
    closesocket(serverSocket);

    return 0;
}
