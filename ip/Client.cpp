#include "stdafx.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Client.h"
#include "Util.h"

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

	char cmd = 'x';
	while (true)
	{
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

		cmd = recvBuffer[0];
		if (cmd == 'e')  // exit
		{
			std::cout << "got cmd = exit" << std::endl;
			break;
		}
		if (cmd == 'i')  // request image
		{
			std::cout << "got cmd = request image" << std::endl;

			char sendBuffer[200] = "s";

			cv::VideoCapture cameraOne(0);
			cv::VideoCapture cameraTwo(2);

			if (!cameraOne.isOpened() || !cameraTwo.isOpened())
			{
				if (!cameraOne.isOpened())
				{
					std::cout << "Could not open camera one!" << std::endl;
				}
				if (!cameraTwo.isOpened())
				{
					std::cout << "Could not open camera two!" << std::endl;
				}
				sendBuffer[0] = 'e';

				// notify UA about error
				sendBuffer[0] = 'e';
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
				continue;
			}

			Mat_<Vec3b> imgOriginalColorOne, imgOriginalColorTwo;

			cameraOne >> imgOriginalColorOne;
			cameraTwo >> imgOriginalColorTwo;

			Mat_<Vec3b> imgResizedColorOne, imgResizedColorTwo;
			resize(imgOriginalColorOne, imgResizedColorOne, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));
			resize(imgOriginalColorTwo, imgResizedColorTwo, cv::Size(IMAGE_WIDTH, IMAGE_HEIGHT));


			imwrite(PATH_IMG_CAM_ONE, imgResizedColorOne);
			imwrite(PATH_IMG_CAM_TWO, imgResizedColorTwo);

			cameraOne.release();
			cameraTwo.release();

			// notify UA about success
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
			continue;
		}
	}

	// close socket
	std::cout << "Calling closesocket" << std::endl;
	closesocket(clientSocket);

	return 0;
}