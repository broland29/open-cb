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
            flat = i + j * 8;
            comboBoxes[flat] = new QComboBox;
            comboBoxes[flat]->addItems(items);
            resultLayout->addWidget(comboBoxes[flat], i, j);
        }
    }

    QWidget* radioWidget = new QWidget;
    QHBoxLayout* radioLayout = new QHBoxLayout(radioWidget);
    radioWidget->setFixedHeight(50);
    radioWidget->setStyleSheet("background-color:magenta");
    trainRadioButton = new QRadioButton("Train");
    trainRadioButton->click();
    testRadioButton = new QRadioButton("Test");
    radioLayout->addWidget(trainRadioButton);
    radioLayout->addWidget(testRadioButton);
    middleRightLayout->addWidget(radioWidget);

    QWidget* actionWidget = new QWidget;
    QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
    actionWidget->setFixedHeight(50);
    actionWidget->setStyleSheet("background-color:darkRed");
    runButton = new QPushButton("Run KNN");
    resetButton = new QPushButton("Reset KNN");
    actionLayout->addWidget(runButton);
    actionLayout->addWidget(resetButton);
    middleRightLayout->addWidget(actionWidget);

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

    connect(runButton, SIGNAL(clicked()), this, SLOT(runButtonClicked()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
    connect(getImageButton, SIGNAL(clicked()), this, SLOT(getImageButtonClicked()));
    connect(sendToVARButton, SIGNAL(clicked()), this, SLOT(sendToVARButtonClicked()));
    connect(helpButton, SIGNAL(clicked()), this, SLOT(helpButtonClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitButtonClicked()));
    //ui.setupUi(this);
}

UserApplicationModule::~UserApplicationModule()
{}

void UserApplicationModule::runButtonClicked()
{
    QString board;
    board.resize(64);
    for (int i = 0; i < 64; i++)
    {
        QString enc = comboBoxes[i]->currentText();  // == overloaded for QString

        if      (enc == "FR") { board[i] = '*'; }
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
            messageLabel->setText("Unknown comboBox value " + enc);
            return;
        }
    }
    messageLabel->setText(board);

    if (trainRadioButton->isChecked())
    {
        emit sendBoardToIPTrainSignal(board);
    }
    
}

void UserApplicationModule::resetButtonClicked()
{
    messageLabel->setText("Reset pressed");
    emit resetKnnSignal();
}

void UserApplicationModule::getImageButtonClicked()
{
    messageLabel->setText("Get image not implemented");
    requestImageSignal();
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
    messageLabel->setText("Exit not implemented");
}

#define PATH_IMG_CAM_ONE "preview\\cam1.jpeg"
#define PATH_IMG_CAM_TWO "preview\\cam2.jpeg"

void UserApplicationModule::setImageSlot()
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
}