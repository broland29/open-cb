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
    
    QObject::connect(&w, &UserApplicationModule::sendToTrainSignal, serverWorker, &Server::sendToTrainSlot);    // sendToTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::sendToTestSignal, serverWorker, &Server::sendToTestSlot);      // sendToTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::runTrainSignal, serverWorker, &Server::runTrainSlot);          // runTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::runTestSignal, serverWorker, &Server::runTestSlot);            // runTestButton clicked
    QObject::connect(&w, &UserApplicationModule::resetTrainSignal, serverWorker, &Server::resetTrainSlot);      // resetTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::resetTestSignal, serverWorker, &Server::resetTestSlot);        // resetTestButton clicked

    QObject::connect(&w, &UserApplicationModule::exitSignal, serverWorker, &Server::exitSlot);



    serverWorker->moveToThread(serverThread);
    serverThread->start();

    int ret = a.exec();
    return ret;
}
