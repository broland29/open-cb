#pragma once

#include <QMainWindow>


class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget* parent = nullptr);
    ~TestWindow();
};
