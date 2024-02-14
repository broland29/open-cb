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
    QObject::connect(&w, &UserApplicationModule::sendToTrainSignalIP, serverWorker, &Server::sendToTrainSlotIP);    // sendToTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::sendToTestSignalIP, serverWorker, &Server::sendToTestSlotIP);      // sendToTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::runTrainSignalIP, serverWorker, &Server::runTrainSlotIP);          // runTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::runTestSignalIP, serverWorker, &Server::runTestSlotIP);            // runTestButton clicked
    QObject::connect(&w, &UserApplicationModule::resetTrainSignalIP, serverWorker, &Server::resetTrainSlotIP);      // resetTrainButton clicked
    QObject::connect(&w, &UserApplicationModule::resetTestSignalIP, serverWorker, &Server::resetTestSlotIP);        // resetTestButton clicked

    QObject::connect(&w, &UserApplicationModule::requestImageSignalIP, serverWorker, &Server::requestImageSlotIP);            // press button, send request to IP
    QObject::connect(serverWorker, &Server::requestImageReplySignalIP, &w, &UserApplicationModule::requestImageReplySlotIP);  // get reply from IP, set preview images and optionally the comboboxes

    QObject::connect(&w, &UserApplicationModule::sendToVARSignalVAR, serverWorker, &Server::sendToVARSlotVAR);              // sendToVARButton pressed
    QObject::connect(serverWorker, &Server::sendToVARReplySignalVAR, &w, &UserApplicationModule::sendToVARReplySlotVAR);    // reply message from VAR

    QObject::connect(&w, &UserApplicationModule::exitSignalIP, serverWorker, &Server::exitSlotIP);
    QObject::connect(&w, &UserApplicationModule::exitSignalVAR, serverWorker, &Server::exitSlotVAR);

    serverWorker->moveToThread(serverThread);
    serverThread->start();

    int ret = a.exec();
    return ret;
}
