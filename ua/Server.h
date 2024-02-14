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

class Server : public QObject
{
    Q_OBJECT

public:
    Server();
    ~Server();
private:
    int socketConnect();
    //int receiveImage();

public slots:
    void doWork();
    void deleteLater();
    
    void sendToTrainSlot(QString board);
    void sendToTestSlot(QString board);
    void runTrainSlot();
    void runTestSlot();
    void resetTrainSlot();
    void resetTestSlot();

    void requestImageSlot(bool classifyWhenGettingImage);
    void exitSlot();

signals:
    void workDone();
    void requestImageReplySignal(QString board);

private:
    SOCKET serverSocket;
    SOCKET acceptSocket;
};
