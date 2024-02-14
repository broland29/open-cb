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
#include <QCheckBox>


class UserApplicationModule : public QMainWindow
{
    Q_OBJECT

public:
    UserApplicationModule(QWidget *parent = nullptr);
    ~UserApplicationModule();
    QString _extractComboBoxes();

private:
    // -- middle -- //
    QLabel* cameraOneImageLabel;
    QLabel* cameraTwoImageLabel;
    std::array<QComboBox*, 64> comboBoxes;
    QPushButton* sendToTrainButton;
    QPushButton* sendToTestButton;
    QPushButton* runTrainButton;
    QPushButton* runTestButton;
    QPushButton* resetTrainButton;
    QPushButton* resetTestButton;
    QCheckBox* classifyWhenGettingImageCheckbox;

    // -- message -- //
    QLabel* messageLabel;

    // -- bottom -- //
    QPushButton* getImageButton;
    QPushButton* sendToVARButton;
    QPushButton* helpButton;
    QPushButton* exitButton;

    Ui::UserApplicationModuleClass ui;

public slots:
    void sendToTrainButtonClicked();
    void sendToTestButtonClicked();
    void runTrainButtonClicked();
    void runTestButtonClicked();
    void resetTrainButtonClicked();
    void resetTestButtonClicked();

    void getImageButtonClicked();
    void sendToVARButtonClicked();
    void helpButtonClicked();
    void exitButtonClicked();

    void requestImageReplySlot(QString board);

signals:
    void sendToTrainSignalIP(QString board);
    void sendToTestSignalIP(QString board);
    void runTrainSignalIP();
    void runTestSignalIP();
    void resetTrainSignalIP();
    void resetTestSignalIP();

    void requestImageSignalIP(bool classifyWhenGettingImage);
    void exitSignalIP();
    void exitSignalVAR();
};
