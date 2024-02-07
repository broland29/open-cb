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
    //qDebug() << ">>>> before\n";
    QApplication a(argc, argv);
    //qDebug() << ">>>> after\n";
    //Wrap* wrap = new Wrap();
    //wrap->start();
    UserApplicationModule w;
    w.show();

    // is putting this here the solution?
    Server* server1 = new Server("server1");
    Server* server2 = new Server("server2");
    Server* server3 = new Server("server3");
    server1->start();  // calls QThread run
    server2->start();  // calls QThread run
    server3->start();  // calls QThread run

    int ret = a.exec();
    return ret;
}
