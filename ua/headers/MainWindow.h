#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include "ClickableLabel.h"
#include "EncodingMapper.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
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
    QPushButton* configureButton;
    QPushButton* getImageButton;
    QPushButton* sendToVARButton;
    QPushButton* getFromVARButton;
    QPushButton* newGameButton;
    QPushButton* helpButton;
    QPushButton* exitButton;

    // clicking logic
    std::array<std::string, 13> pieceNames = { "FR", "WP", "WB", "WN", "WR", "WQ", "WK", "BP", "BB", "BN", "BR", "BQ", "BK" };
    std::map<std::string, QPixmap> nameToPixmap;
    int lastRow = -1;
    int lastCol = -1;
    std::string lastPieceName;

    void _setInitialSetup();

public slots:
    // right buttons, clicked slots
    void sendToTrainButtonClicked();
    void sendToTestButtonClicked();
    void runTrainButtonClicked();
    void runTestButtonClicked();
    void resetTrainButtonClicked();
    void resetTestButtonClicked();

    // bottom buttons, clicked slots
    void configureButtonClicked();
    void getImageButtonClicked();
    void sendToVARButtonClicked();
    void getFromVARButtonClicked();
    void newGameButtonClicked();
    void helpButtonClicked();
    void exitButtonClicked();

    // right buttons, IP -> UA
    void sendToTrainReplySlot(bool succeeded, QString message);
    void sendToTestReplySlot(bool succeeded, QString message);
    void runTrainReplySlot(bool succeeded, QString message);
    void runTestReplySlot(bool succeeded, QString message);
    void resetTrainReplySlot(bool succeeded, QString message);
    void resetTestReplySlot(bool succeeded, QString message);

    // bottom buttons, IP/VAR -> UA
    void configureReplySlot(bool succeeded, QString message);
    void getImageReplySlot(bool succeeded, QString message);
    void sendToVARReplySlot(QString message);
    void getFromVARReplySlot(QString board);
    void newGameReplySlot(bool succeded);
    void exitReplySlot(bool succeded);

    // clicks on chess GUI
    void leftClickedSlot(int row, int col, std::string pieceName);
    void rightClickedSlot(int row, int col, std::string pieceName);

    // getting a new frame
    void imageUpdateSlotOne(QImage image);
    void imageUpdateSlotTwo(QImage image);

signals:
    // right buttons, UA -> IP
    void sendToTrainSignal(QString board);
    void sendToTestSignal(QString board);
    void runTrainSignal();
    void runTestSignal();
    void resetTrainSignal();
    void resetTestSignal();

    // bottom buttons, UA -> IP/VAR
    void configureSignal();
    void getImageSignal(bool classifyWhenGettingImage);
    void sendToVARSignal(QString board);
    void getFromVARSignal();
    void newGameSignal();
    void helpSignal();
    void exitSignal();
};
