#include "Server.h"

#pragma warning(disable : 4996)

Server::Server()
{
    // initialize WSA
    WORD wVersionRequested = MAKEWORD(2, 2);        // we will use version 2.2
    WSADATA wsaData;                                // structure which will be populated by WSAStartup()
    int wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0)
    {
        qDebug() << "WSAStartup() error, code: " << wsaErr;
        throw 1;
    }
    qDebug() << "WSAStartup() success, status: " << wsaData.szSystemStatus;

    // create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        qDebug() << "socket() error: " << WSAGetLastError();
        WSACleanup();
        throw 1;
    }
    qDebug() << "socket() success.";

    // bind socket to ip and port
    sockaddr_in service;  // will be casted to older SOCKADDR type
    service.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &service.sin_addr);
    service.sin_port = htons(55555);

    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        qDebug() << "bind() error: " << WSAGetLastError();
        closesocket(serverSocket);
        WSACleanup();
        throw 1;
    }
    qDebug() << "bind() success.";

    // initially invalid
    acceptSocket = NULL;
}

Server::~Server()
{

}

int Server::socketConnect()
{
    // listen for (one) client connection
    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        qDebug() << "listen() error: " << WSAGetLastError();
        return 1;
    }
    else
    {
        qDebug() << "listen() success, waiting for connections.";
    }

    // accept: blocking wait for a connection
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET)
    {
        qDebug() << "accept() failed: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }
    qDebug() << "accept() success";

    return 0;
}

// hidden primitive
int _sendMessage(SOCKET acceptSocket, char message[SEND_BUFFER_SIZE])
{
    assert(acceptSocket != NULL);
    int sendByteCount = send(acceptSocket, message, SEND_BUFFER_SIZE, 0);
    if (sendByteCount > 0)
    {
        std::cout << "send() success." << std::endl;
    }
    else
    {
        std::cout << "send() error: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    return 0;
}

// hidden primitive
int _receiveMessage(SOCKET acceptSocket, char message[RECV_BUFFER_SIZE])
{
    assert(acceptSocket != NULL);
    char recvBuffer[RECV_BUFFER_SIZE];

    int recvByteCount = recv(acceptSocket, recvBuffer, RECV_BUFFER_SIZE, 0);
    if (recvByteCount > 0)
    {
        std::cout << "recv() recieved: " << recvBuffer << std::endl;
    }
    else
    {
        std::cout << "recv() error: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    strcpy(message, recvBuffer);
    return 0;
}


void Server::requestImageSlot()
{
    qDebug() << "entered requestImage";
    char requestImageMessage[] = "i";
    _sendMessage(acceptSocket, requestImageMessage);
    char recvBuffer[RECV_BUFFER_SIZE];
    _receiveMessage(acceptSocket, recvBuffer);
    
    if (recvBuffer[0] == 's')
    {
        qDebug() << "got success";
        setImageSignal();
    }
    else
    {
        qDebug() << "did not get success";
    }
}

void Server::doWork()
{
    socketConnect();
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;
    /*
    int i = 0;
    while (true)
    {
        qDebug() << i++;
        QThread::sleep(1);
    }
    */
}

void Server::sendToTrainSlot(QString board)
{
    board.prepend("t");
    QByteArray byteArray = board.toLatin1();
    char* msg = byteArray.data();
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::sendToTestSlot(QString board)
{
    board.prepend("T");
    QByteArray byteArray = board.toLatin1();
    char* msg = byteArray.data();
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::runTrainSlot()
{
    char msg[4] = "x";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::runTestSlot()
{
    char msg[4] = "X";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::resetTrainSlot()
{
    char msg[4] = "r";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::resetTestSlot()
{
    char msg[4] = "R";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::exitSlot()
{
    char msg[2] = "e";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(acceptSocket, msg);
}

void Server::deleteLater()
{

}



/*
Server::Server(const char name[10])
{
    strncpy(this->name, name, 10);
    
    // initialize WSA
    WORD wVersionRequested = MAKEWORD(2, 2);        // we will use version 2.2
    WSADATA wsaData;                                // structure which will be populated by WSAStartup()
    int wsaErr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaErr != 0)
    {
        std::cout << "WSAStartup() error, code: " << wsaErr << std::endl;
        throw 1;
    }
    std::cout << "WSAStartup() success, status: " << wsaData.szSystemStatus << std::endl;

    // create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cout << "socket() error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        throw 1;
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
        throw 1;
    }
    std::cout << "bind() success." << std::endl;

    // initially invalid
    acceptSocket = NULL;
    
}


int Server::connect()
{
    // listen for (one) client connection
    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        std::cout << "listen() error: " << WSAGetLastError() << std::endl;
        return 1;
    }
    else
    {
        std::cout << "listen() success, waiting for connections." << std::endl;
    }

    // accept: blocking wait for a connection
    acceptSocket = accept(serverSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET)
    {
        std::cout << "accept() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "accept() success" << std::endl;

    return 0;
}


// hidden primitive
int _receiveMessage(SOCKET acceptSocket, char message[RECV_BUFFER_SIZE])
{
    assert(acceptSocket != NULL);
    char recvBuffer[RECV_BUFFER_SIZE];

    int recvByteCount = recv(acceptSocket, recvBuffer, RECV_BUFFER_SIZE, 0);
    if (recvByteCount > 0)
    {
        std::cout << "recv() recieved: " << recvBuffer << std::endl;
    }
    else
    {
        std::cout << "recv() error: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    strcpy(message, recvBuffer);
    return 0;
}


int Server::receiveMessage(char board[8][8], char& cmd)
{
    assert(acceptSocket != NULL);
    char recvBuffer[RECV_BUFFER_SIZE];
    int ret = _receiveMessage(acceptSocket, recvBuffer);
    cmd = recvBuffer[0];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            board[i][j] = recvBuffer[i * 8 + j + 1];
        }
    }
    return ret;
}


// hidden primitive
int _sendMessage(SOCKET acceptSocket, char message[SEND_BUFFER_SIZE])
{
    assert(acceptSocket != NULL);
    int sendByteCount = send(acceptSocket, message, SEND_BUFFER_SIZE, 0);
    if (sendByteCount > 0)
    {
        std::cout << "send() success." << std::endl;
    }
    else
    {
        std::cout << "send() error: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }

    return 0;
}


int Server::sendMessage(char message[SEND_BUFFER_SIZE])
{
    assert(acceptSocket != NULL);
    return _sendMessage(acceptSocket, message);
}


int Server::close()
{
    std::cout << "Calling closesocket" << std::endl;
    closesocket(serverSocket);
    closesocket(acceptSocket);
    return 0;
}


void Server::run()
{
    //connect();
    while (true)
    //for (int i = 0; i < 10; i++)
    {
        qDebug() << name << "\n";//": " << i << "\n";
        //QThread::sleep(100);
    }
}
*/


