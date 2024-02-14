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
    IPSocket = NULL;
    VARSocket = NULL;
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
    IPSocket = accept(serverSocket, NULL, NULL);
    if (IPSocket == INVALID_SOCKET)
    {
        qDebug() << "accept() for IP failed: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }
    qDebug() << "accept() for IP success";

    VARSocket = accept(serverSocket, NULL, NULL);
    if (VARSocket == INVALID_SOCKET)
    {
        qDebug() << "accept() for VAR failed: " << WSAGetLastError();
        WSACleanup();
        return 1;
    }
    qDebug() << "accept() for VAR success";

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

/* communication with IP */

void Server::sendToTrainSlotIP(QString board)
{
    board.prepend("t");
    QByteArray byteArray = board.toLatin1();
    char* msg = byteArray.data();
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::sendToTestSlotIP(QString board)
{
    board.prepend("T");
    QByteArray byteArray = board.toLatin1();
    char* msg = byteArray.data();
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::runTrainSlotIP()
{
    char msg[4] = "x";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::runTestSlotIP()
{
    char msg[4] = "X";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::resetTrainSlotIP()
{
    char msg[4] = "r";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::resetTestSlotIP()
{
    char msg[4] = "R";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::requestImageSlotIP(bool classifyWhenGettingImage)
{
    qDebug() << "entered requestImage";

    char msg[1];
    if (classifyWhenGettingImage)
    {
        msg[0] = 'I';
    }
    else
    {
        msg[0] = 'i';
    }
    _sendMessage(IPSocket, msg);

    char recvBuffer[RECV_BUFFER_SIZE];
    _receiveMessage(IPSocket, recvBuffer);

    if (recvBuffer[0] != 's')
    {
        qDebug() << "did not get success";
        return;
    }

    QString board;
    board.resize(64);
    if (classifyWhenGettingImage)
    {
        char boardChar[65];
        strncpy(boardChar, recvBuffer + 1, 64);
        boardChar[64] = '\0';
        board = QString::fromLatin1(boardChar);
    }

    // notify GUI that preview can be loaded from predefined path
    requestImageReplySignalIP(board);
}

void Server::exitSlotIP()
{
    char msg[2] = "e";
    qDebug() << "Will send " << msg << " to IP";
    _sendMessage(IPSocket, msg);
}

void Server::exitSlotVAR()
{
    char msg[2] = "e";
    qDebug() << "Will send " << msg << " to VAR";
    _sendMessage(VARSocket, msg);
}

void Server::deleteLater()
{

}