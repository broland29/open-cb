#pragma once

#include "ui_UserApplicationModule.h"

#include <QtWidgets/QMainWindow>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>


class UserApplicationModule : public QMainWindow
{
    Q_OBJECT

public:
    UserApplicationModule(QWidget *parent = nullptr);
    ~UserApplicationModule();

private:
    // -- middle -- //
    QLabel* cameraOneImageLabel;
    QLabel* cameraTwoImageLabel;
    std::array<QComboBox*, 64> comboBoxes;
    QRadioButton* trainRadioButton;
    QRadioButton* testRadioButton;
    QPushButton* runButton;
    QPushButton* resetButton;

    // -- message -- //
    QLabel* messageLabel;

    // -- bottom -- //
    QPushButton* getImageButton;
    QPushButton* sendToVARButton;
    QPushButton* helpButton;
    QPushButton* exitButton;

    Ui::UserApplicationModuleClass ui;

public slots:
    void runButtonClicked();
    void resetButtonClicked();
    void getImageButtonClicked();
    void sendToVARButtonClicked();
    void helpButtonClicked();
    void exitButtonClicked();
};
