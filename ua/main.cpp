#include "UserApplicationModule.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserApplicationModule w;
    w.show();
    return a.exec();
}
