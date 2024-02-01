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
    QLabel* label;
    QRadioButton* radioButton1;
    QRadioButton* radioButton2;
    QRadioButton* radioButton3;
    QCheckBox* checkBox1;
    QCheckBox* checkBox2;
    QCheckBox* checkBox3;
    QLineEdit* priceLineEdit;
    QLineEdit* totalLineEdit;
    QSpinBox* spinBox;
    QLCDNumber* lcdNumber;
    QComboBox* comboBox;
    QSlider* slider;

    Ui::UserApplicationModuleClass ui;

public slots:
    void ChangeText();
    void ChangeRadio();
    void ChangeCheck();
    void ChangeSpin();
    void showTime();
    void randomGenerator();
    void comboChanged();
    void changeSlider();
};
