#include "Client.h"


void Client::validateNewBoard(char recvBuffer[200])
{
	char board[8][8];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			board[i][j] = recvBuffer[i * 8 + j + 1];
		}
	}

	char sendBuffer[200];
	validator->validateBoard(board, sendBuffer);

	int sendByteCount = send(clientSocket, sendBuffer, 200, 0);
	if (sendByteCount > 0)
	{
		SPDLOG_INFO("Sending validator response succeeded.");
	}
	else
	{
		SPDLOG_INFO("Sending validator response failed! Error {0:d}.", WSAGetLastError());
		WSACleanup();
	}
}


void Client::getCurrentBoard()
{
	char sendBuffer[200];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			sendBuffer[i * 8 + j] = validator->currBoard[i][j];
		}
	}
	sendBuffer[64] = '\0';

	int sendByteCount = send(clientSocket, sendBuffer, 200, 0);
	if (sendByteCount > 0)
	{
		SPDLOG_INFO("Sending current board succeeded.");
	}
	else
	{
		SPDLOG_INFO("Sending current board failed! Error {0:d}.", WSAGetLastError());
		WSACleanup();
	}
}


void Client::startNewGame()
{
	validator = new Validator;
	char sendBuffer[] = "s";

	int sendByteCount = send(clientSocket, sendBuffer, 200, 0);
	if (sendByteCount > 0)
	{
		SPDLOG_INFO("Sending response succeeded.");
	}
	else
	{
		SPDLOG_INFO("Sending response failed! Error {0:d}.", WSAGetLastError());
		WSACleanup();
	}
}


Client::Client()
{
	// initialize WSA
	WSADATA wsaData;  // structure which will be populated by WSAStartup()
	WORD wVersionRequested = MAKEWORD(2, 2);  // we will use version 2.2
	int wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0)
	{
		SPDLOG_ERROR("Initializing WSA failed! Error {0:d}.", wsaErr);
		throw 1;
	}
	SPDLOG_INFO("Initializing WSA succeeded. Status {}.", wsaData.szSystemStatus);


	// create socket
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		SPDLOG_ERROR("Socket creation failed! Error {0:d}.", WSAGetLastError());
		WSACleanup();
		throw 1;
	}
	SPDLOG_INFO("Socket creation successful.");


	// connect to server
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(SERVER_PORT);  // server port
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		SPDLOG_ERROR("Connection to server failed! Error {0:d}.", WSAGetLastError());
		WSACleanup();
		throw 1;
	}
	SPDLOG_INFO("Connection to server successful.");


	// initialize validator
	validator = new Validator;
}

int Client::runLoop()
{
	while (true)
	{
		// recieve message
		char recvBuffer[200];
		int recvByteCount = recv(clientSocket, recvBuffer, 200, 0);
		if (recvByteCount > 0)
		{
			SPDLOG_INFO("Recieve from server successful. Received {}.", recvBuffer);
		}
		else
		{
			SPDLOG_INFO("Recieve from server unsuccessful! Error {}.", WSAGetLastError());
			WSACleanup();
			continue;
		}

		switch (recvBuffer[0])
		{
		case 'e':
			SPDLOG_INFO("Got command e, will exit.");
			break;
		case 'b':
			SPDLOG_INFO("Got command b, will validate new board.");
			validateNewBoard(recvBuffer);
			continue;
		case 'g':
			SPDLOG_INFO("Got command g, will get current board.");
			getCurrentBoard();
			continue;
		case 'n':
			SPDLOG_INFO("Got command n, will start new game.");
			startNewGame();
			continue;
		default:
			SPDLOG_INFO("Unknown command {}.", recvBuffer[0]);
			continue;
		}
		break;

		// in the loop, nothing should follow the switch, since cases might end with:
		//		- continue: the loop will go on
		//		- break: the switch finishes and gets to second break
		// https://stackoverflow.com/questions/1420029/how-to-break-out-of-a-loop-from-inside-a-switch
	}

	// close socket
	SPDLOG_INFO("Ending loop, closing socket.");
	closesocket(clientSocket);
	return 0;
}
