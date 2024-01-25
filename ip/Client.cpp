#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Client.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55555



int client_main()
{
	// initialize WSA
	WSADATA wsaData;  // structure which will be populated by WSAStartup()
	WORD wVersionRequested = MAKEWORD(2, 2);  // we will use version 2.2
	int wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0)
	{
		std::cout << "WSAStartup() error, code: " << wsaErr << std::endl;
		return 1;
	}
	std::cout << "WSAStartup() success, status: " << wsaData.szSystemStatus << std::endl;

	// create socket
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "socket() error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	std::cout << "socket() success." << std::endl;

	// connect to server
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(SERVER_PORT);  // server port
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		std::cout << "connect() error: " << WSAGetLastError();
		WSACleanup();
		return 1;
	}
	std::cout << "connect() success." << std::endl;

	// send message
	char sendBuffer[200] = "Message from client";
	int sendByteCount = send(clientSocket, sendBuffer, 200, 0);
	if (sendByteCount > 0)
	{
		std::cout << "send() success." << std::endl;
	}
	else
	{
		std::cout << "send() error: " << WSAGetLastError();
		WSACleanup();
	}

	// recieve message
	char recvBuffer[200];
	int recvByteCount = recv(clientSocket, recvBuffer, 200, 0);
	if (recvByteCount > 0)
	{
		std::cout << "recv() recieved: " << recvBuffer << std::endl;
	}
	else
	{
		std::cout << "recv() error: " << WSAGetLastError();
		WSACleanup();
	}

	// close socket
	std::cout << "Calling closesocket" << std::endl;
	closesocket(clientSocket);

	return 0;
}