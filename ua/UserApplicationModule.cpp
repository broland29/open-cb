#include "UserApplicationModule.h"

UserApplicationModule::UserApplicationModule(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("OpenCB");
    setFixedSize(800, 700);

    QWidget *centralWidget = new QWidget;
    QVBoxLayout* centralLayout = new QVBoxLayout();

    // -- top -- //
    QWidget* topWidget = new QWidget;
    QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
    QLabel* topLabel = new QLabel("OpenCB User Application Beta");
    topLayout->addWidget(topLabel, 0, Qt::AlignCenter);
    topWidget->setStyleSheet("background-color:yellow");
    topWidget->setFixedHeight(50);


    // -- middle -- //
    QWidget* middleWidget = new QWidget;
    QHBoxLayout* middleLayout = new QHBoxLayout(middleWidget);

    // middle left
    QWidget* middleLeftWidget = new QWidget;
    QVBoxLayout* middleLeftLayout = new QVBoxLayout(middleLeftWidget);
    middleLeftWidget->setFixedWidth(250);
    middleLeftWidget->setStyleSheet("background-color:green");

    QPixmap cameraOnePlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    cameraOneImageLabel = new QLabel();
    cameraOneImageLabel->setFixedWidth(230);
    cameraOneImageLabel->setPixmap(cameraOnePlaceholder.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraOneImageLabel);
    QLabel* cameraOneTextLabel = new QLabel("Camera one");
    middleLeftLayout->addWidget(cameraOneTextLabel);

    QPixmap cameraTwoPlaceholder = QPixmap(":/UserApplicationModule/placeholder.jpeg");
    cameraTwoImageLabel = new QLabel();
    cameraTwoImageLabel->setFixedWidth(230);
    cameraTwoImageLabel->setPixmap(cameraTwoPlaceholder.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));
    middleLeftLayout->addWidget(cameraTwoImageLabel);
    QLabel* cameraTwoTextLabel = new QLabel("Camera two");
    middleLeftLayout->addWidget(cameraTwoTextLabel);

    // middle right
    QWidget* middleRightWidget = new QWidget;
    QVBoxLayout* middleRightLayout = new QVBoxLayout(middleRightWidget);
    middleRightWidget->setFixedWidth(450);
    middleRightWidget->setStyleSheet("background-color:darkGreen");

    QWidget* resultWidget = new QWidget;
    QGridLayout* resultLayout = new QGridLayout(resultWidget);
    resultWidget->setFixedWidth(430);
    resultWidget->setFixedHeight(300);
    resultWidget->setStyleSheet("background-color:white");
    middleRightLayout->addWidget(resultWidget, 0, Qt::AlignHCenter);

    QStringList items = (QStringList() << "FR" << "WP" << "WB" << "WN" << "WR" << "WQ" << "WK" << "BP" << "BB" << "BN" << "BR" << "BQ" << "BK");
    int flat;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            flat = i * 8 + j;
            comboBoxes[flat] = new QComboBox;
            comboBoxes[flat]->addItems(items);
            resultLayout->addWidget(comboBoxes[flat], i, j);
        }
    }

    QWidget* actionWidget = new QWidget;
    QGridLayout* actionLayout = new QGridLayout(actionWidget);
    actionWidget->setFixedHeight(130);
    actionWidget->setStyleSheet("background-color:darkRed");
    sendToTrainButton = new QPushButton("Send to Train");
    sendToTestButton = new QPushButton("Send to Test");
    runTrainButton = new QPushButton("Run Train");
    runTestButton = new QPushButton("Run Test");
    resetTrainButton = new QPushButton("Reset Train");
    resetTestButton = new QPushButton("Reset Test");
    actionLayout->addWidget(sendToTrainButton, 0, 0);
    actionLayout->addWidget(sendToTestButton, 0, 1);
    actionLayout->addWidget(runTrainButton, 1, 0);
    actionLayout->addWidget(runTestButton, 1, 1);
    actionLayout->addWidget(resetTrainButton, 2, 0);
    actionLayout->addWidget(resetTestButton, 2, 1);

    QWidget* checkboxWidget = new QWidget;
    QHBoxLayout* checkboxLayout = new QHBoxLayout(checkboxWidget);
    classifyWhenGettingImageCheckbox = new QCheckBox("Classify when getting image");
    checkboxLayout->addWidget(classifyWhenGettingImageCheckbox, 0, Qt::AlignCenter);
    //checkboxWidget->setFixedHeight(50);

    middleRightLayout->addWidget(actionWidget);
    middleRightLayout->addWidget(checkboxWidget);

    middleLayout->addWidget(middleLeftWidget);
    middleLayout->addWidget(middleRightWidget);
    middleWidget->setStyleSheet("background-color:blue");
    middleWidget->setFixedHeight(500);

    // -- message -- //
    QWidget* messageWidget = new QWidget;
    QHBoxLayout* messageLayout = new QHBoxLayout(messageWidget);
    messageLabel = new QLabel("message");
    messageLayout->addWidget(messageLabel);
    messageWidget->setStyleSheet("background-color:cyan");
    messageWidget->setFixedHeight(50);

    // -- bottom -- //
    QWidget* bottomWidget = new QWidget;
    QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
    getImageButton = new QPushButton("Get image");
    sendToVARButton = new QPushButton("Send to VAR");
    helpButton = new QPushButton("Help");
    exitButton = new QPushButton("Exit");
    bottomLayout->addWidget(getImageButton);
    bottomLayout->addWidget(sendToVARButton);
    bottomLayout->addWidget(helpButton);
    bottomLayout->addWidget(exitButton);
    bottomWidget->setStyleSheet("background-color:gray");
    bottomWidget->setFixedHeight(50);

    centralLayout->addWidget(topWidget);
    centralLayout->addWidget(middleWidget);
    centralLayout->addWidget(messageWidget);
    centralLayout->addWidget(bottomWidget);

    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);

    connect(sendToTrainButton, SIGNAL(clicked()), this, SLOT(sendToTrainButtonClicked()));
    connect(sendToTestButton, SIGNAL(clicked()), this, SLOT(sendToTestButtonClicked()));
    connect(runTrainButton, SIGNAL(clicked()), this, SLOT(runTrainButtonClicked()));
    connect(runTestButton, SIGNAL(clicked()), this, SLOT(runTestButtonClicked()));
    connect(resetTrainButton, SIGNAL(clicked()), this, SLOT(resetTrainButtonClicked()));
    connect(resetTestButton, SIGNAL(clicked()), this, SLOT(resetTestButtonClicked()));

    connect(getImageButton, SIGNAL(clicked()), this, SLOT(getImageButtonClicked()));
    connect(sendToVARButton, SIGNAL(clicked()), this, SLOT(sendToVARButtonClicked()));
    connect(helpButton, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitButtonClicked()));
    //ui.setupUi(this);
}

UserApplicationModule::~UserApplicationModule()
{}

QString UserApplicationModule::_extractComboBoxes()
{
    QString board;
    board.resize(64);
    for (int i = 0; i < 64; i++)
    {
        QString enc = comboBoxes[i]->currentText();  // == overloaded for QString

        if (enc == "FR") { board[i] = '*'; }
        else if (enc == "WP") { board[i] = 'P'; }
        else if (enc == "WB") { board[i] = 'B'; }
        else if (enc == "WN") { board[i] = 'N'; }
        else if (enc == "WR") { board[i] = 'R'; }
        else if (enc == "WQ") { board[i] = 'Q'; }
        else if (enc == "WK") { board[i] = 'K'; }
        else if (enc == "BP") { board[i] = 'p'; }
        else if (enc == "BB") { board[i] = 'b'; }
        else if (enc == "BN") { board[i] = 'n'; }
        else if (enc == "BR") { board[i] = 'r'; }
        else if (enc == "BQ") { board[i] = 'q'; }
        else if (enc == "BK") { board[i] = 'k'; }
        else
        {
            return "Unknown comboBox value " + enc;
        }
    }
    return board;
}

void UserApplicationModule::sendToTrainButtonClicked()
{
    messageLabel->setText("sendToTrainButtonClicked");
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToTrainSignalIP(board);
}

void UserApplicationModule::sendToTestButtonClicked()
{
    messageLabel->setText("sendToTestButtonClicked");
    QString board = _extractComboBoxes();
    messageLabel->setText(board);
    emit sendToTestSignalIP(board);
}

void UserApplicationModule::runTrainButtonClicked()
{
    messageLabel->setText("runTrainButtonClicked");
    emit runTrainSignalIP();
}

void UserApplicationModule::runTestButtonClicked()
{
    messageLabel->setText("runTestButtonClicked");
    emit runTestSignalIP();
}

void UserApplicationModule::resetTrainButtonClicked()
{
    messageLabel->setText("resetTrainButtonClicked");
    emit resetTrainSignalIP();
}

void UserApplicationModule::resetTestButtonClicked()
{
    messageLabel->setText("resetTestButtonClicked");
    emit resetTestSignalIP();
}

void UserApplicationModule::getImageButtonClicked()
{
    messageLabel->setText("getImageButtonClicked");
    emit requestImageSignalIP(classifyWhenGettingImageCheckbox->isChecked());
}

void UserApplicationModule::sendToVARButtonClicked()
{
    messageLabel->setText("Send to VAR not implemented");
}

void UserApplicationModule::helpButtonClicked()
{
    messageLabel->setText("Help not implemented");
}

void UserApplicationModule::exitButtonClicked()
{
    emit exitSignalIP();
    emit exitSignalVAR();
    QCoreApplication::quit();
}

#define PATH_IMG_CAM_ONE "preview\\cam1.jpeg"
#define PATH_IMG_CAM_TWO "preview\\cam2.jpeg"

void UserApplicationModule::requestImageReplySlot(QString board)
{
    messageLabel->setText("Set image fired! Yippie!");

    QImage cameraOneImage, cameraTwoImage;
    if (cameraOneImage.load(PATH_IMG_CAM_ONE) == false)
    {
        messageLabel->setText("Could not load" + QString::fromUtf8(PATH_IMG_CAM_ONE));
        return;
    }
    if (cameraTwoImage.load(PATH_IMG_CAM_TWO) == false)
    {
        messageLabel->setText("Could not load" + QString::fromUtf8(PATH_IMG_CAM_TWO));
        return;
    }

    QPixmap cameraOnePixmap = QPixmap::fromImage(cameraOneImage);
    QPixmap cameraTwoPixmap = QPixmap::fromImage(cameraTwoImage);

    cameraOneImageLabel->setPixmap(cameraOnePixmap.scaled(cameraOneImageLabel->width(), cameraOneImageLabel->height(), Qt::KeepAspectRatio));
    cameraTwoImageLabel->setPixmap(cameraTwoPixmap.scaled(cameraTwoImageLabel->width(), cameraTwoImageLabel->height(), Qt::KeepAspectRatio));

    for (int i = 0; i < 64; i++)
    {
        if (board[i] == '*') { comboBoxes[i]->setCurrentText("FR"); }
        else if (board[i] == 'P') { comboBoxes[i]->setCurrentText("WP"); }
        else if (board[i] == 'B') { comboBoxes[i]->setCurrentText("WB"); }
        else if (board[i] == 'N') { comboBoxes[i]->setCurrentText("WN"); }
        else if (board[i] == 'R') { comboBoxes[i]->setCurrentText("WR"); }
        else if (board[i] == 'Q') { comboBoxes[i]->setCurrentText("WQ"); }
        else if (board[i] == 'K') { comboBoxes[i]->setCurrentText("WK"); }
        else if (board[i] == 'p') { comboBoxes[i]->setCurrentText("BP"); }
        else if (board[i] == 'b') { comboBoxes[i]->setCurrentText("BB"); }
        else if (board[i] == 'n') { comboBoxes[i]->setCurrentText("BN"); }
        else if (board[i] == 'r') { comboBoxes[i]->setCurrentText("BR"); }
        else if (board[i] == 'q') { comboBoxes[i]->setCurrentText("BQ"); }
        else if (board[i] == 'k') { comboBoxes[i]->setCurrentText("BK"); }
        else
        {
            messageLabel->setText(QString("Unknown board value ") + board[i]);
            return;
        }
    }
}