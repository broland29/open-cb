#include "../headers/TestWindow.h"

TestWindow::TestWindow(QWidget* parent) :
    QMainWindow(parent)
{
    setWindowTitle("Test");
    setFixedSize(500, 500);
}

TestWindow::~TestWindow()
{
}