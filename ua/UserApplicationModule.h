#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UserApplicationModule.h"
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLCDNumber>
#include <QComboBox>
#include <QSlider>
//#include <QListWidget>

class UserApplicationModule : public QMainWindow
{
    Q_OBJECT

public:
    UserApplicationModule(QWidget *parent = nullptr);
    ~UserApplicationModule();

private:
    Ui::UserApplicationModuleClass ui;

public slots:

};
