#include "UserApplicationModule.h"
#include "Server.h"
#include <QtWidgets/QApplication>

class Wrap : public QThread
{
    void run()
    {
        UserApplicationModule w;
        w.show();
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserApplicationModule w;
    w.show();

    // create server thread
    QThread* serverThread = new QThread;
    Server* serverWorker = new Server;
    QObject::connect(serverThread, &QThread::started, serverWorker, &Server::doWork);
    QObject::connect(serverWorker, &Server::workDone, serverThread, &QThread::quit);
    QObject::connect(serverThread, &QThread::finished, serverWorker, &Server::deleteLater);

    // cross - thread communication
    QObject::connect(&w, &UserApplicationModule::requestImageSignal, serverWorker, &Server::requestImageSlot);  // press button, send request to IP
    QObject::connect(serverWorker, &Server::setImageSignal, &w, &UserApplicationModule::setImageSlot);  // get response from IP, set preview images
    QObject::connect(&w, &UserApplicationModule::sendBoardToIPTrainSignal, serverWorker, &Server::sendBoardToIPTrainSlot);  // runKNN train
    QObject::connect(&w, &UserApplicationModule::resetKnnSignal, serverWorker, &Server::resetKnnSlot);  // press resetKnn button

    serverWorker->moveToThread(serverThread);
    serverThread->start();

    int ret = a.exec();
    return ret;
}
