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

#include "ClickableLabel.h"


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
    std::array<std::array<ClickableLabel*, 8>, 8> pieceLabels;
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

    // clicking logic
    std::array<std::string, 13> pieceNames = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
    std::map<std::string, QPixmap> nameToPixmap;
    int lastRow = -1;
    int lastCol = -1;
    std::string lastPieceName;

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

    void requestImageReplySlotIP(QString board);
    void sendToVARReplySlotVAR(QString message);

    void leftClickedSlot(int row, int col, std::string pieceName);
    void rightClickedSlot(int row, int col, std::string pieceName);


signals:
    void sendToTrainSignalIP(QString board);
    void sendToTestSignalIP(QString board);
    void runTrainSignalIP();
    void runTestSignalIP();
    void resetTrainSignalIP();
    void resetTestSignalIP();

    void requestImageSignalIP(bool classifyWhenGettingImage);
    void sendToVARSignalVAR(QString board);
    void exitSignalIP();
    void exitSignalVAR();
};
