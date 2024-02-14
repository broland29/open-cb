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
    
    void sendToTrainSlotIP(QString board);
    void sendToTestSlotIP(QString board);
    void runTrainSlotIP();
    void runTestSlotIP();
    void resetTrainSlotIP();
    void resetTestSlotIP();

    void requestImageSlotIP(bool classifyWhenGettingImage);
    void sendToVARSlotVAR(QString board);
    void exitSlotIP();
    void exitSlotVAR();

signals:
    void workDone();
    void requestImageReplySignalIP(QString board);
    void sendToVARReplySignalVAR(QString message);

private:
    SOCKET serverSocket;
    SOCKET IPSocket;
    SOCKET VARSocket;
};
